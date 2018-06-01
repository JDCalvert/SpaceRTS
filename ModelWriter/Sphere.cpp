#include "Sphere.h"

Sphere::Sphere(unsigned int detail)
{
    this->detail = detail;
}

std::string Sphere::getFileName()
{
    return "sphere" + std::to_string(detail);
}

void Sphere::build()
{
    double PI = 4 * atan(1.0);

    unsigned int nLatitude = detail;
    unsigned int nLongitude = detail * 2;

    for (unsigned int latitude = 0; latitude <= nLatitude; latitude++)
    {
        double phi = PI / 2 - latitude * PI / nLatitude;
        for (unsigned int longitude = 0; longitude <= nLongitude; longitude++)
        {
            double theta = -PI + longitude * 2 * PI / nLongitude;
            double x = cos(theta) * cos(phi);
            double y = sin(theta) * cos(phi);
            double z = sin(phi);

            float u = (float)longitude / nLongitude;
            float v = (float)latitude / nLatitude;

            Vertex vertex = Vertex{glm::vec3(x, y, z), glm::vec2(u, v), glm::vec3(x, y, z),  mainBoneDependency};
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
}