#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
#include <fstream>
#include <sstream>

struct Bone
{
    glm::mat4 relative;
    int parent = -1;

    Bone() {}
    Bone(glm::mat4 relative, int parent)
    {
        this->relative = relative;
        this->parent = parent;
    }

};

struct Vertex
{
    glm::vec3 vertex;
    glm::vec2 textureCoordinate;
    glm::vec3 normal;
    glm::vec4 boneIndices;
};

struct Surface
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::vector<Bone> bones = {Bone()};
};

std::string addTriangle(Surface& surface, int parentBoneIndex);
std::string addCube(Surface& surface, int parentBoneIndex);
std::string addSphere(Surface& surface, int parentBoneIndex, int detail);
std::string addCylinder(Surface& surface, int detail);
std::string addCapitalShip(Surface& surface, int parentBoneIndex);

void writeToBinaryFile(Surface& surface, std::string fileName);
void calculateTangents(Surface& surface, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

void main()
{
    std::vector<Surface*> surfaces;
    std::vector<std::string> fileNames;

    Surface triangleSurface;
    fileNames.push_back(addTriangle(triangleSurface, 0));
    surfaces.push_back(&triangleSurface);

    Surface cubeSurface;
    fileNames.push_back(addCube(cubeSurface, 0));
    surfaces.push_back(&cubeSurface);

    Surface sphereSurface;
    fileNames.push_back(addSphere(sphereSurface, 0, 20));
    surfaces.push_back(&sphereSurface);

    Surface capitalShipSurface;
    fileNames.push_back(addCapitalShip(capitalShipSurface, 0));
    surfaces.push_back(&capitalShipSurface);

    Surface cylinderSurface;
    fileNames.push_back(addCylinder(cylinderSurface, 30));
    surfaces.push_back(&cylinderSurface);

    for (unsigned int i = 0; i < surfaces.size(); i++)
    {
        Surface* surface = surfaces[i];
        std::string fileName = fileNames[i];

        writeToBinaryFile(*surface, fileName);
    }
}

std::string addTriangle(Surface &surface, int parentBoneIndex)
{
    int vertexIndex = surface.vertices.size();
    int boneIndex = surface.bones.size();

    double pi = 4 * atan(1);

    glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 middleLeft = glm::vec3(0.25f, 0.0f, 0.0f);
    glm::vec3 middleRight = glm::vec3(-0.25, 0.0f, 0.0f);
    glm::vec3 backLeft = glm::vec3(0.5f, 0.0f, -1.0f);
    glm::vec3 backRight = glm::vec3(-0.5f, 0.0f, -1.0f);

    glm::vec2 frontTexCoord = glm::vec2(0.5f, 0.0f);
    glm::vec2 middleLeftTexCoord = glm::vec2(0.25f, 0.5f);
    glm::vec2 middleRightTexCoord = glm::vec2(0.75f, 0.5f);
    glm::vec2 backLeftTexCoord = glm::vec2(0.0f, 1.0f);
    glm::vec2 backRightTexCoord = glm::vec2(1.0f, 1.0f);

    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

    Bone frontBone{glm::translate(glm::vec3(0.0f, 0.0f, 1.0f)), parentBoneIndex};

    surface.bones.push_back(frontBone);

    surface.vertices.push_back(Vertex{front,		frontTexCoord,		 normal, glm::vec4(boneIndex + 0, 0.5, parentBoneIndex,  0.5)});
    surface.vertices.push_back(Vertex{middleLeft,  middleLeftTexCoord,  normal, glm::vec4(boneIndex + 0, 0.5, parentBoneIndex,  0.5)});
    surface.vertices.push_back(Vertex{middleRight, middleRightTexCoord, normal, glm::vec4(boneIndex + 0, 0.5, parentBoneIndex,  0.5)});
    surface.vertices.push_back(Vertex{middleLeft,  middleLeftTexCoord,  normal, glm::vec4(parentBoneIndex, 1.0, -1, -1)});
    surface.vertices.push_back(Vertex{middleRight, middleRightTexCoord, normal, glm::vec4(parentBoneIndex, 1.0, -1, -1)});
    surface.vertices.push_back(Vertex{backLeft,	backLeftTexCoord,	 normal, glm::vec4(parentBoneIndex, 1.0, -1, -1)});
    surface.vertices.push_back(Vertex{backRight,   backRightTexCoord,   normal, glm::vec4(parentBoneIndex, 1.0, -1, -1)});

    surface.indices.push_back(vertexIndex + 0);
    surface.indices.push_back(vertexIndex + 1);
    surface.indices.push_back(vertexIndex + 2);
    surface.indices.push_back(vertexIndex + 3);
    surface.indices.push_back(vertexIndex + 5);
    surface.indices.push_back(vertexIndex + 6);
    surface.indices.push_back(vertexIndex + 3);
    surface.indices.push_back(vertexIndex + 6);
    surface.indices.push_back(vertexIndex + 4);

    return "triangle";
}

void addPanel(Surface &surface, glm::vec3 topLeft, glm::vec3 bottomLeft, glm::vec3 topRight, glm::vec3 bottomRight,
    glm::vec2 topLeftTexCoord, glm::vec2 bottomLeftTexCoord, glm::vec2 topRightTexCoord, glm::vec2 bottomRightTexCoord,
    glm::vec3 normal, glm::vec4 boneDependency)
{
    int index = surface.vertices.size();

    surface.vertices.push_back(Vertex{topLeft,		topLeftTexCoord,	normal, boneDependency});
    surface.vertices.push_back(Vertex{bottomLeft,	bottomLeftTexCoord,	normal, boneDependency});
    surface.vertices.push_back(Vertex{topRight,	topRightTexCoord,	normal, boneDependency});
    surface.vertices.push_back(Vertex{bottomRight,	bottomRightTexCoord,normal, boneDependency});

    surface.indices.push_back(index + 0);
    surface.indices.push_back(index + 1);
    surface.indices.push_back(index + 2);
    surface.indices.push_back(index + 2);
    surface.indices.push_back(index + 1);
    surface.indices.push_back(index + 3);
}

std::string addCube(Surface &surface, int parentBoneIndex)
{
    double pi = 4 * atan(1);

    glm::vec3 bottomRightBack = glm::vec3(-1.0, -1.0, -1.0);
    glm::vec3 bottomRightFront = glm::vec3(-1.0, -1.0, 1.0);
    glm::vec3 topRightBack = glm::vec3(-1.0, 1.0, -1.0);
    glm::vec3 topRightFront = glm::vec3(-1.0, 1.0, 1.0);

    glm::vec3 bottomLeftBack = glm::vec3(1.0, -1.0, -1.0);
    glm::vec3 bottomLeftFront = glm::vec3(1.0, -1.0, 1.0);
    glm::vec3 topLeftBack = glm::vec3(1.0, 1.0, -1.0);
    glm::vec3 topLeftFront = glm::vec3(1.0, 1.0, 1.0);

    glm::vec2 topLeftTexCoord = glm::vec2(0.0, 0.0);
    glm::vec2 bottomLeftTexCoord = glm::vec2(0.0, 1.0);
    glm::vec2 topRightTexCoord = glm::vec2(1.0, 0.0);
    glm::vec2 bottomRightTexCoord = glm::vec2(1.0, 1.0);

    glm::vec3 normalBottom = glm::vec3(0.0, -1.0, 0.0);
    glm::vec3 normalTop = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 normalLeft = glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 normalFront = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 normalRight = glm::vec3(-1.0, 0.0, 0.0);
    glm::vec3 normalBack = glm::vec3(0.0, 0.0, -1.0);

    glm::vec4 centreBoneDependency = glm::vec4(parentBoneIndex, 1, -1, -1);

    addPanel(surface, topRightBack, bottomRightBack, topRightFront, bottomRightFront, topLeftTexCoord, bottomLeftTexCoord, topRightTexCoord, bottomRightTexCoord, normalRight, centreBoneDependency);
    addPanel(surface, topRightFront, bottomRightFront, topLeftFront, bottomLeftFront, topLeftTexCoord, bottomLeftTexCoord, topRightTexCoord, bottomRightTexCoord, normalFront, centreBoneDependency);
    addPanel(surface, topLeftFront, bottomLeftFront, topLeftBack, bottomLeftBack, topLeftTexCoord, bottomLeftTexCoord, topRightTexCoord, bottomRightTexCoord, normalLeft, centreBoneDependency);
    addPanel(surface, topLeftBack, bottomLeftBack, topRightBack, bottomRightBack, topLeftTexCoord, bottomLeftTexCoord, topRightTexCoord, bottomRightTexCoord, normalBack, centreBoneDependency);
    addPanel(surface, topLeftFront, topLeftBack, topRightFront, topRightBack, topLeftTexCoord, bottomLeftTexCoord, topRightTexCoord, bottomRightTexCoord, normalTop, centreBoneDependency);
    addPanel(surface, bottomLeftBack, bottomLeftFront, bottomRightBack, bottomRightFront, topLeftTexCoord, bottomLeftTexCoord, topRightTexCoord, bottomRightTexCoord, normalBottom, centreBoneDependency);

    return "cube";
}

std::string addSphere(Surface& surface, int parentBoneIndex, int detail)
{
    double PI = 3.14159;

    unsigned int nLatitude = detail;
    unsigned int nLongitude = detail * 2;

    glm::vec4 parentBoneDependency(parentBoneIndex, 1.0, -1, -1);

    for (unsigned int latitude = 0; latitude <= nLatitude; latitude++)
    {
        double phi = PI / 2 - latitude * PI / nLatitude;
        for (unsigned int longitude = 0; longitude <= nLongitude; longitude++)
        {
            double theta = (-PI + longitude * 2 * PI / nLongitude);
            double x = cos(theta) * cos(phi);
            double y = sin(phi);
            double z = sin(theta) * cos(phi);

            float u = (float)longitude / nLongitude;
            float v = (float)latitude / nLatitude;

            Vertex vertex = Vertex{glm::vec3(x, y, z), glm::vec2(u, v), glm::vec3(x, y, z),  parentBoneDependency};
            surface.vertices.push_back(vertex);
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

            surface.indices.push_back(topLeft);
            surface.indices.push_back(bottomLeft);
            surface.indices.push_back(topRight);
            surface.indices.push_back(topRight);
            surface.indices.push_back(bottomLeft);
            surface.indices.push_back(bottomRight);
        }
    }

    return "Sphere";
}

std::string addCylinder(Surface& surface, int detail)
{
    double PI = atan(1.0) * 4;

    glm::vec4 parentBoneDependency(0, 1.0f, -1, -1);

    int numVertices = 1 + detail + (detail+1) + (detail+1) + detail + 1;
    surface.vertices.resize(numVertices);

    int numTriangles = detail * 4;
    surface.indices.resize(numTriangles * 3);

    unsigned int topMidOffset = 0;
    unsigned int topUpOffset = topMidOffset + 1;
    unsigned int bottomMidOffset = topUpOffset + detail;
    unsigned int bottomDownOffset = bottomMidOffset + 1;
    unsigned int outOffset = bottomDownOffset + detail;

    unsigned int topIndicesOffset = 0;
    unsigned int bottomIndicesOffset = topIndicesOffset + detail * 3;
    unsigned int outIndicesOffset = bottomIndicesOffset + detail * 3;

    Vertex topMid =
    {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec2(0.5f, 0.5f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        parentBoneDependency
    };

    Vertex bottomMid = 
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec2(0.5f, 0.5f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        parentBoneDependency
    };

    surface.vertices[topMidOffset] = topMid;
    surface.vertices[bottomMidOffset] = bottomMid;

    for (unsigned int i=0; i<=detail; i++)
    {
        double theta = -PI + i * (2 * PI / detail);

        float x = cos(theta);
        float y = sin(theta);

        float u = (float)i / detail;

        if (i < detail)
        {
            Vertex topUp =
            {
                glm::vec3(x, y, 1.0f),
                glm::vec2((x+1)/2, (y+1/2)),
                glm::vec3(0.0f, 0.0f, 1.0f),
                parentBoneDependency
            };

            Vertex bottomDown =
            {
                glm::vec3(x, y, 0.0f),
                glm::vec2((x+1)/2, (y+1)/2),
                glm::vec3(0.0f, 0.0f, -1.0f),
                parentBoneDependency
            };

            surface.vertices[topUpOffset + i] = topUp;
            surface.vertices[bottomDownOffset + i] = bottomDown;

            unsigned int topIndex0 = topMidOffset;
            unsigned int topIndex1 = topUpOffset + i;
            unsigned int topIndex2 = topUpOffset + i + 1;
            if (topIndex2 == bottomMidOffset)
            {
                topIndex2 = topUpOffset;
            }

            surface.indices[topIndicesOffset + i*3] = topIndex0;
            surface.indices[topIndicesOffset + i*3+1] = topIndex1;
            surface.indices[topIndicesOffset + i*3+2] = topIndex2;

            unsigned int bottomIndex0 = bottomMidOffset;
            unsigned int bottomIndex1 = bottomDownOffset + i;
            unsigned int bottomIndex2 = bottomDownOffset + i + 1;
            if (bottomIndex2 == outOffset)
            {
                bottomIndex2 = bottomDownOffset;
            }

            surface.indices[bottomIndicesOffset + i*3] = bottomIndex0;
            surface.indices[bottomIndicesOffset + i*3+1] = bottomIndex1;
            surface.indices[bottomIndicesOffset + i*3+2] = bottomIndex2;
        }

        Vertex topOut = 
        {
            glm::vec3(x, y, 1.0f),
            glm::vec2(u, 0.0f),
            glm::vec3(x, y, 0.0f),
            parentBoneDependency
        };

        Vertex bottomOut = 
        {
            glm::vec3(x, y, 0.0f),
            glm::vec2(u, 1.0f),
            glm::vec3(x, y, 0.0f),
            parentBoneDependency
        };

        surface.vertices[outOffset + i*2] = topOut;
        surface.vertices[outOffset + i*2 + 1] = bottomOut;

        if (i < detail)
        {
            unsigned int quadTopLeft        = outOffset + i*2;
            unsigned int quadBottomLeft     = outOffset + i*2 + 1;
            unsigned int quadTopRight       = outOffset + i*2 + 2;
            unsigned int quadBottomRight    = outOffset + i*2 + 3;

            unsigned int indexOffset = outIndicesOffset + i*6;

            surface.indices[indexOffset] = quadTopLeft;
            surface.indices[indexOffset + 1] = quadBottomLeft;
            surface.indices[indexOffset + 2] = quadTopRight;
            surface.indices[indexOffset + 3] = quadTopRight;
            surface.indices[indexOffset + 4] = quadBottomLeft;
            surface.indices[indexOffset + 5] = quadBottomRight;
        }
    }

    return "Cylinder" + detail;
}

std::string addTriangle(Surface& surface, int parentBoneIndex, glm::vec3 position0, glm::vec3 position1, glm::vec3 position2,
    glm::vec2 textureCoordinate0, glm::vec2 textureCoordinate1, glm::vec2 textureCoordinate2)
{
    int index = surface.vertices.size();

    glm::vec4 parentBoneDependency(parentBoneIndex, 1.0, -1, -1);
    glm::vec3 normal = glm::normalize(glm::cross(position1 - position0, position2 - position0));

    glm::vec3 positions[] = {position0, position1, position2};
    glm::vec2 textureCoordinates[] = {textureCoordinate0, textureCoordinate1, textureCoordinate2};

    for (unsigned int i = 0; i < 3; i++)
    {
        Vertex vertex = Vertex{positions[i], textureCoordinates[i], normal, parentBoneDependency};
        surface.vertices.push_back(vertex);
        surface.indices.push_back(index + i);
    }

    return "SimpleTriangle";
}

std::string addCapitalShip(Surface& surface, int parentBoneIndex)
{
    glm::vec3 frontMiddleTop = glm::vec3(0.0, 0.05, 8.0);
    glm::vec3 backMiddleTop = glm::vec3(0.0, 1.30, -8.0);
    glm::vec3 backLeftTop = glm::vec3(4.4, 0.05, -6.0);
    glm::vec3 backRightTop = glm::vec3(-4.4, 0.05, -6.0);
    glm::vec3 frontMiddleBottom = glm::vec3(0.0, -0.05, 8.0);
    glm::vec3 backMiddleBottom = glm::vec3(0.0, -1.20, -8.0);
    glm::vec3 backRightBottom = glm::vec3(-4.4, -0.05, -6.0);
    glm::vec3 backLeftBottom = glm::vec3(4.4, -0.05, -6.0);

    glm::vec2 frontMiddleTexCoord = glm::vec2(0.5, 0.0);
    glm::vec2 frontRightTexCoord = glm::vec2(1.0, 0.0);
    glm::vec2 frontLeftTexCoord = glm::vec2(0.0, 0.0);
    glm::vec2 backLeftTexCoord = glm::vec2(0.0, 1.0);
    glm::vec2 backRightTexCoord = glm::vec2(1.0, 1.0);
    glm::vec2 backMiddleTexCoord = glm::vec2(0.5, 1.0);

    glm::vec3 leftNormal = glm::normalize(glm::cross(frontMiddleBottom - frontMiddleTop, backMiddleTop - frontMiddleTop));
    glm::vec3 rightNormal = glm::normalize(glm::cross(backRightBottom - backRightTop, frontMiddleTop - backRightTop));
    glm::vec3 backNormal = glm::vec3(0.0, 0.0, -1.0);

    glm::vec4 boneDependency = glm::vec4(parentBoneIndex, 1.0, -1, -1);

    addTriangle(surface, parentBoneIndex, frontMiddleTop, backLeftTop, backMiddleTop, frontMiddleTexCoord, backLeftTexCoord, backMiddleTexCoord);
    addTriangle(surface, parentBoneIndex, frontMiddleTop, backMiddleTop, backRightTop, frontMiddleTexCoord, backMiddleTexCoord, backRightTexCoord);
    addTriangle(surface, parentBoneIndex, frontMiddleBottom, backMiddleBottom, backLeftBottom, frontMiddleTexCoord, backMiddleTexCoord, backLeftTexCoord);
    addTriangle(surface, parentBoneIndex, frontMiddleBottom, backRightBottom, backMiddleBottom, frontMiddleTexCoord, backRightTexCoord, backMiddleTexCoord);
    addTriangle(surface, parentBoneIndex, backMiddleTop, backLeftTop, backRightTop, frontMiddleTexCoord, backLeftTexCoord, backRightTexCoord);
    addTriangle(surface, parentBoneIndex, backMiddleBottom, backRightBottom, backLeftBottom, backMiddleTexCoord, frontRightTexCoord, frontLeftTexCoord);
    addPanel(surface, frontMiddleTop, frontMiddleBottom, backLeftTop, backLeftBottom, frontRightTexCoord, frontLeftTexCoord, backRightTexCoord, backLeftTexCoord, leftNormal, boneDependency);
    addPanel(surface, backRightTop, backRightBottom, frontMiddleTop, frontMiddleBottom, backLeftTexCoord, backRightTexCoord, frontLeftTexCoord, frontRightTexCoord, rightNormal, boneDependency);
    addPanel(surface, backLeftTop, backLeftBottom, backRightTop, backRightBottom, glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0), glm::vec2(176, 0.0), glm::vec2(176, 1.0), backNormal, boneDependency);

    return "CapitalShip";
}

std::string addDualTurret(Surface& surface, int parentBoneIndex)
{
    //First, add a cylinder for the turrent to sit on
    //addCylinder(surface, parentBoneIndex, 0.05f, 0.01f, 30);

    //Add a bone for the body, sitting at the top and centre of the cylinder
    Bone bodyBone = Bone{glm::translate(glm::vec3(0.0f, 0.01f, 1.0f)), parentBoneIndex};

    glm::vec3 bodyRightTopFront = glm::vec3(-0.03f, 0.07f, 0.025f);
    glm::vec3 bodyLeftTopFront = glm::vec3(0.03f, 0.07f, 0.025f);
    glm::vec3 bodyLeftBottomFront = glm::vec3(0.03f, 0.01f, 0.035f);
    glm::vec3 bodyRightBottomFront = glm::vec3(-0.03f, 0.01f, 0.035f);
    glm::vec3 bodyRightTopBack = glm::vec3(-0.025f, 0.04f, -0.03f);
    glm::vec3 bodyLeftTopBack = glm::vec3(0.025f, 0.04f, -0.03f);
    glm::vec3 bodyLeftBottomBack = glm::vec3(0.025f, 0.01f, -0.03f);
    glm::vec3 bodyRightBottomBack = glm::vec3(-0.025f, 0.01f, -0.03f);

    glm::vec2 frontRightTexCoord = glm::vec2(1.0, 0.0);
    glm::vec2 frontLeftTexCoord = glm::vec2(0.0, 0.0);
    glm::vec2 backLeftTexCoord = glm::vec2(0.0, 1.0);
    glm::vec2 backRightTexCoord = glm::vec2(1.0, 1.0);

    return "Turret";
}

void writeToBinaryFile(Surface& surface, std::string fileName)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> boneIndices;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (unsigned int i = 0; i < surface.vertices.size(); i++)
    {
        Vertex& vertex = surface.vertices[i];

        vertices.push_back(vertex.vertex);
        textureCoordinates.push_back(vertex.textureCoordinate);
        normals.push_back(vertex.normal);
        boneIndices.push_back(vertex.boneIndices);
    }

    calculateTangents(surface, tangents, bitangents);

    std::vector<glm::mat4> bones;
    std::vector<int> parents;
    for (unsigned int i = 0; i < surface.bones.size(); i++)
    {
        Bone bone = surface.bones[i];
        bones.push_back(bone.relative);
        parents.push_back(bone.parent);
    }

    std::vector<unsigned int> &indices = surface.indices;
    std::vector<unsigned int> sizes = {vertices.size(), indices.size(), bones.size()};

    int sizesSize = sizes.size() * sizeof(unsigned int);
    int verticesSize = vertices.size() * sizeof(glm::vec3);
    int textureCoordinatesSize = textureCoordinates.size() * sizeof(glm::vec2);
    int normalsSize = normals.size() * sizeof(glm::vec3);
    int tangentsSize = tangents.size() * sizeof(glm::vec3);
    int bitangentsSize = bitangents.size() * sizeof(glm::vec3);
    int boneIndicesSize = boneIndices.size() * sizeof(glm::vec4);
    int indicesSize = indices.size() * sizeof(unsigned int);
    int bonesSize = bones.size() * sizeof(glm::mat4);
    int parentsSize = parents.size() * sizeof(int);

    int verticesOffset = sizesSize;
    int textureCoordinatesOffset = verticesOffset + verticesSize;
    int normalsOffset = textureCoordinatesOffset + textureCoordinatesSize;
    int tangentsOffset = normalsOffset + normalsSize;
    int bitangentsOffset = tangentsOffset + tangentsSize;
    int boneIndicesOffset = bitangentsOffset + bitangentsSize;
    int indicesOffset = boneIndicesOffset + boneIndicesSize;
    int bonesOffset = indicesOffset + indicesSize;
    int parentsOffset = bonesOffset + bonesSize;
    int totalSize = parentsOffset + parentsSize;

    char* data = new char[totalSize];
    memcpy(&data[0], &sizes[0], sizesSize);
    memcpy(&data[verticesOffset], &vertices[0], verticesSize);
    memcpy(&data[textureCoordinatesOffset], &textureCoordinates[0], textureCoordinatesSize);
    memcpy(&data[normalsOffset], &normals[0], normalsSize);
    memcpy(&data[tangentsOffset], &tangents[0], tangentsSize);
    memcpy(&data[bitangentsOffset], &bitangents[0], bitangentsSize);
    memcpy(&data[boneIndicesOffset], &boneIndices[0], boneIndicesSize);
    memcpy(&data[indicesOffset], &indices[0], indicesSize);
    memcpy(&data[bonesOffset], &bones[0], bonesSize);
    memcpy(&data[parentsOffset], &parents[0], parentsSize);

    std::ofstream objFile(fileName.append(".objComplete"), std::ios::out | std::ios::binary);
    objFile.write(data, totalSize);
    objFile.close();
}

void calculateTangents(Surface& surface, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
{
    unsigned int verticesLength = surface.vertices.size();
    tangents.resize(verticesLength);
    bitangents.resize(verticesLength);

    std::vector<unsigned int> &indices = surface.indices;

    for (unsigned int i = 0; i<indices.size(); i += 3)
    {
        int index0 = indices[i];
        int index1 = indices[i + 1];
        int index2 = indices[i + 2];

        Vertex &vertex0 = surface.vertices[index0];
        Vertex &vertex1 = surface.vertices[index1];
        Vertex &vertex2 = surface.vertices[index2];

        glm::vec3 deltaPos1 = vertex1.vertex - vertex0.vertex;
        glm::vec3 deltaPos2 = vertex2.vertex - vertex0.vertex;

        glm::vec2 deltaTextureCoordinate1 = vertex1.textureCoordinate - vertex0.textureCoordinate;
        glm::vec2 deltaTextureCoordinate2 = vertex2.textureCoordinate - vertex0.textureCoordinate;

        float r = 1.0f / (deltaTextureCoordinate1.x * deltaTextureCoordinate2.y - deltaTextureCoordinate1.y * deltaTextureCoordinate2.x);
        glm::vec3 tangent = (deltaPos1 * deltaTextureCoordinate2.y - deltaPos2 * deltaTextureCoordinate1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaTextureCoordinate1.x - deltaPos1 * deltaTextureCoordinate2.x) * r;

        tangents[index0] += tangent;
        tangents[index1] += tangent;
        tangents[index2] += tangent;
        bitangents[index0] += bitangent;
        bitangents[index1] += bitangent;
        bitangents[index2] += bitangent;
    }
}