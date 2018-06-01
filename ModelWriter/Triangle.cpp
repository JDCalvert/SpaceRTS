#include "Triangle.h"

std::string Triangle::getFileName()
{
    return "triangle";
}

void Triangle::build()
{
    double PI = atan(1.0) * 4;

    glm::vec3 vertexPositions[3] =
    {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(sin(4 * PI / 3), cos(4 * PI / 3), 0.0f),
        glm::vec3(sin(2 * PI / 3), cos(2 * PI / 3), 0.0f)
    };

    glm::vec2 textureCoordinates[3];
    for (unsigned int i=0; i<3; i++)
    {
        glm::vec3 vertexPosition = vertexPositions[i];
        textureCoordinates[i] = glm::vec2((vertexPosition.x + 1.0f) / 2, (1.0f - vertexPosition.y) / 2);
    }

    glm::vec3 normal(0.0f, 0.0f, 1.0f);

    addTriangle(vertexPositions, textureCoordinates, mainBoneDependency);
}