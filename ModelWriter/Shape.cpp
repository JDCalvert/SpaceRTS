#include "Shape.h"

Shape::Shape() : Surface()
{
    bones.push_back(Bone(glm::mat4(), -1));

    mainBoneDependency = glm::vec4(0, 1.0f, -1, 0.0f);
}

void Shape::buildAndWrite()
{
    build();

    std::vector<glm::vec3>& surfaceVertices = getVertices();
    std::vector<glm::vec2>& textureCoordinates = getTextureCoordinates();
    std::vector<glm::vec3>& normals = getNormals();
    std::vector<glm::vec4>& boneIndicesAndWeights = getBoneIndicesAndWeights();

    unsigned int numVertices = vertices.size();
    surfaceVertices.resize(numVertices);
    textureCoordinates.resize(numVertices);
    normals.resize(numVertices);
    boneIndicesAndWeights.resize(numVertices);

    for (unsigned int i=0; i<numVertices; i++)
    {
        Vertex vertex = vertices[i];

        surfaceVertices[i] = vertex.vertex;
        textureCoordinates[i] = vertex.textureCoordinate;
        normals[i] = vertex.normal;
        boneIndicesAndWeights[i] = vertex.boneIndicesAndWeights;
    }

    getIndices() = indices;
    getBones() = bones;

    calculateSizesAndLength();

    std::string fileName = "../Resources/Models/" + getFileName() + ".mesh";
    writeToFile(fileName.c_str());
}

void Shape::addTriangle(glm::vec3 vertexPosition0, glm::vec3 vertexPosition1, glm::vec3 vertexPosition2,
                        glm::vec2 textureCoordinate0, glm::vec2 textureCoordinate1, glm::vec2 textureCoordinate2,
                        glm::vec4 boneDependency)
{
    unsigned int baseIndex = vertices.size();

    glm::vec3 normal = glm::cross(vertexPosition1 - vertexPosition0, vertexPosition2 - vertexPosition0);

    vertices.push_back(Vertex{vertexPosition0, textureCoordinate0, normal, boneDependency});
    vertices.push_back(Vertex{vertexPosition1, textureCoordinate1, normal, boneDependency});
    vertices.push_back(Vertex{vertexPosition2, textureCoordinate2, normal, boneDependency});

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
}

void Shape::addTriangle(glm::vec3 vertexPositions[3], glm::vec2 textureCoordinates[3], glm::vec4 boneDependency)
{
    addTriangle(vertexPositions[0], vertexPositions[1], vertexPositions[2],
                textureCoordinates[0], textureCoordinates[1], textureCoordinates[2],
                boneDependency);
}

void Shape::addQuad(glm::vec3 topLeft, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 topRight,
                    glm::vec2 topLeftTexCoord, glm::vec2 bottomLeftTexCoord, glm::vec2 bottomRightTexCoord, glm::vec2 topRightTexCoord,
                    glm::vec3 normal, glm::vec4 boneDependency)
{
    unsigned int baseIndex = vertices.size();

    vertices.push_back(Vertex{topLeft,		topLeftTexCoord,	normal, boneDependency});
    vertices.push_back(Vertex{bottomLeft,  	bottomLeftTexCoord,	normal, boneDependency});
    vertices.push_back(Vertex{bottomRight,	bottomRightTexCoord,normal, boneDependency});
    vertices.push_back(Vertex{topRight,     topRightTexCoord,	normal, boneDependency});

    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}