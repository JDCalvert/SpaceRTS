#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

bool operator > (glm::vec2 point1, glm::vec2 point2)
{
    return point1.x > point2.x
        && point1.y > point2.y;
}

bool operator < (glm::vec2 point1, glm::vec2 point2)
{
    return point1.x < point2.x
        && point1.y < point2.y;
}

class Geometry
{
public:

    static bool isPointInBox(glm::vec2 pointPosition, glm::vec2 boxPosition, glm::vec2 boxSize)
    {
        return pointPosition > boxPosition
            && pointPosition < boxPosition + boxSize;
    }

    static void constructFrustum(float(&collisionPlanes)[6][4], glm::vec3(&frustumPoints)[8])
    {
        int indices[18] = {0, 1, 3, 0, 4, 1, 0, 3, 4, 6, 5, 7, 6, 7, 2, 6, 2, 5};

        //Make six planes, described by their normal vector and the distance from the origin, to represent the frustum (all plane normals face inwards)
        for (int i = 0; i<6; i++)
        {
            int index0 = indices[i * 3];
            int index1 = indices[i * 3 + 1];
            int index2 = indices[i * 3 + 2];

            glm::vec3 vec0 = frustumPoints[index0];
            glm::vec3 vec1 = frustumPoints[index1];
            glm::vec3 vec2 = frustumPoints[index2];

            glm::vec3 vecI = vec1 - vec0;
            glm::vec3 vecJ = vec2 - vec0;

            glm::vec3 normal = glm::normalize(glm::cross(vecI, vecJ));

            collisionPlanes[i][0] = normal[0];
            collisionPlanes[i][1] = normal[1];
            collisionPlanes[i][2] = normal[2];
            collisionPlanes[i][3] = glm::dot(normal, vec0);
        }
    }

    static bool isLineInFrustum(glm::vec4 end1, glm::vec4 end2, float(&planes)[6][4])
    {
        for (int i = 0; i<6; i++)
        {
            glm::vec4 planeNormal = glm::vec4(planes[i][0], planes[i][1], planes[i][2], 0.0f);

            //Work out which of the ends are on the "inside" of the plane (the side of the plane facing the inside of the frustum
            bool end1In = glm::dot(planeNormal, end1) < planes[i][3];
            bool end2In = glm::dot(planeNormal, end2) < planes[i][3];

            //If neither of the ends are "inside" then the line doesn't go through the frustum
            if (!end1In && !end2In)
            {
                return false;
            }

            //Find the intersection point, and trim the line so only the part "inside" the plane is left
            if (!end1In || !end2In)
            {
                float numerator = planes[i][3] - glm::dot(end1, planeNormal);
                glm::vec4 line = end2 - end1;
                float denominator = glm::dot(line, planeNormal);
                float d = numerator / denominator;
                glm::vec4 intersectionPoint = end1 + d * line;
                if (end1In)
                {
                    end2 = intersectionPoint;
                }
                else
                {
                    end1 = intersectionPoint;
                }
            }
        }

        return true;
    }

    static float getPointToLineDistance(glm::vec4 x0, glm::vec4 x1, glm::vec4 x2)
    {
        glm::vec4 x0x1 = x1 - x0;
        glm::vec4 x1x2 = x2 - x1;

        glm::vec3 cross = glm::cross(glm::vec3(x1x2), glm::vec3(x0x1));
        float crossLength = glm::length(cross);
        float x1x2Length = glm::length(x1x2);

        return crossLength / x1x2Length;
    }

    static bool isLineInSphere(glm::vec4 position, glm::vec4 end1, glm::vec4 end2, float radius)
    {
        return getPointToLineDistance(position, end1, end2) <= radius;
    }

    static bool getLineEllipsoidIntersection(glm::vec4 end1, glm::vec4 end2, float xRadius, float yRadius, float zRadius, glm::vec4 &intersection)
    {
        float x = end1.x;
        float y = end1.y;
        float z = end1.z;

        float dx = end2.x - x;
        float dy = end2.y - y;
        float dz = end2.z - z;

        float xRadiusSquared = xRadius * xRadius;
        float yRadiusSquared = yRadius * yRadius;
        float zRadiusSquared = zRadius * zRadius;

        //The formular for an ellipsoid is x^2/rx^2 + y^2/ry^2 + z^2/rz^2 = 1. Substitute the equation for the line (p = end1 + (end2 - end1)*t)
        //and rearrange for t gets a quadratic in the form a*t^2 + b*t + c = 0. Find the solutions (if they exist) with the quadratic equation
        float a = ((dx * dx) / xRadiusSquared) + ((dy * dy) / yRadiusSquared) + ((dz * dz) / zRadiusSquared);
        float b = 2 * (((x * dx) / xRadiusSquared) + ((y * dy) / yRadiusSquared) + ((z * dz) / zRadiusSquared));
        float c = ((x * x) / xRadiusSquared) + ((y * y) / yRadiusSquared) + ((z * z) / zRadiusSquared) - 1;

        //If b^2 - 4ac is negative, then it has no real roots, so the line does not intersect with the ellipsoid at any point
        float root = (b * b) - (4 * a * c);
        if (root < 0)
        {
            return false;
        }

        //We will use the negative root, as this is where the line enters the ellipsoid
        float t = (-b - sqrt(root)) / (2 * a);

        //If t is not between 0 (the first end is on the ellipsoid) and 1 (the second end is on the ellipsoid) then the line intersects
        //but the line segment we're interested in does not
        if (t < 0 || t > 1)
        {
            return false;
        }

        //If t is in between 0 and 1 then the intersection is t way along the line segment between end1 and end2
        intersection = end1 + (end2 - end1) * t;
        return true;
    }

    static bool getLineSphereIntersection(glm::vec3 end1, glm::vec3 end2, glm::vec4 &intersection)
    {
        //If end1 is inside or end2 is outside, it's not a collision; we're only interested in things entering the sphere.
        if (glm::length(end1) < 1 || glm::length(end2) > 1)
        {
            return false;
        }

        glm::vec3 direction = glm::normalize(end2 - end1);

        float b = 2 * glm::dot(direction, end1);
        float c = glm::dot(end1, end1) - 1;

        float root = b * b - 4 * c;
        if (root < 0)
        {
            return false;
        }

        float t = (-b - sqrt(root)) / 2;

        //The line does intersect, but not between the start and end positions
        float length = glm::length((end2 - end1));
        if (t < 0 || t > length)
        {
            return false;
        }

        intersection = glm::vec4(end1 + direction * t, 1.0);
        return true;
    }

    static void getUnitSphere(std::vector<glm::vec3> &vertices, GLuint &verticesSize, std::vector<unsigned int> &indices, GLuint &indicesSize, unsigned int &numIndices)
    {
        double PI = 3.14159;

        unsigned int nLatitude = 30;
        unsigned int nLongitude = nLatitude * 2;

        for (unsigned int latitude = 0; latitude <= nLatitude; latitude++)
        {
            double phi = PI / 2 - latitude * PI / nLatitude;
            for (unsigned int longitude = 0; longitude <= nLongitude; longitude++)
            {
                double theta = (-PI + longitude * 2 * PI / nLongitude);
                double x = cos(theta) * cos(phi);
                double y = sin(phi);
                double z = sin(theta) * cos(phi);

                glm::vec3 vertex = glm::vec3(x, y, z);
                vertices.push_back(vertex);
            }
        }

        for (unsigned int latitude = 0; latitude<nLatitude; latitude++)
        {
            for (unsigned int longitude = 0; longitude<nLongitude; longitude++)
            {
                int topLeft = latitude * (nLongitude + 1) + longitude;
                int bottomLeft = (latitude + 1)*(nLongitude + 1) + longitude;
                int topRight = latitude * (nLongitude + 1) + (longitude + 1);
                int bottomRight = (latitude + 1)*(nLongitude + 1) + (longitude + 1);
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        verticesSize = vertices.size() * sizeof(glm::vec3);
        indicesSize = indices.size() * sizeof(unsigned int);
        numIndices = indices.size();
    }

    static void buildPanel(std::vector<glm::vec2> &vertices, glm::vec2 topLeft, float width, float height)
    {
        glm::vec2 topRight = topLeft + glm::vec2(width, 0);
        glm::vec2 bottomLeft = topLeft + glm::vec2(0, height);
        glm::vec2 bottomRight = topLeft + glm::vec2(width, height);

        vertices.push_back(topLeft);
        vertices.push_back(bottomLeft);
        vertices.push_back(topRight);
        vertices.push_back(topRight);
        vertices.push_back(bottomLeft);
        vertices.push_back(bottomRight);
    }
};

#endif GEOMETRY_H