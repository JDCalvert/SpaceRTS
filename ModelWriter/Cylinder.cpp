#include "Cylinder.h"

Cylinder::Cylinder(unsigned int detail) : Shape()
{
    this->detail = detail;
}

std::string Cylinder::getFileName()
{
    return "cylinder" + std::to_string(detail);
}

void Cylinder::build()
{
    double PI = atan(1.0) * 4;

    int numVertices = 1 + detail + (detail + 1) + (detail + 1) + detail + 1;
    int numTriangles = detail * 4;
    
    shapeVertices.resize(numVertices);
    indices.resize(numTriangles * 3);

    unsigned int topMidOffset = 0;
    unsigned int topUpOffset = topMidOffset + 1;
    unsigned int bottomMidOffset = topUpOffset + detail;
    unsigned int bottomDownOffset = bottomMidOffset + 1;
    unsigned int outOffset = bottomDownOffset + detail;

    unsigned int topIndicesOffset = 0;
    unsigned int bottomIndicesOffset = topIndicesOffset + detail * 3;
    unsigned int outIndicesOffset = bottomIndicesOffset + detail * 3;

    shapeVertices[topMidOffset] =
    {
        glm::vec3(0.00f, 0.00f, 1.00f),
        glm::vec2(0.25f, 0.25f),
        glm::vec3(0.00f, 0.00f, 1.00f),
        mainBoneDependency
    };

    shapeVertices[bottomMidOffset] =
    {
        glm::vec3(0.00f, 0.00f, 0.00f),
        glm::vec2(0.75f, 0.25f),
        glm::vec3(0.00f, 0.00f,-1.00f),
        mainBoneDependency
    };

    for (unsigned int i = 0; i <= detail; i++)
    {
        double theta = -PI + i * (2 * PI / detail);

        float x = cos(theta);
        float y = sin(theta);

        float u = (float)i / detail;

        if (i < detail)
        {
            shapeVertices[topUpOffset + i] =
            {
                glm::vec3(x, y, 1.0f),
                glm::vec2((x + 1) / 4, (y + 1) / 4),
                glm::vec3(0.0f, 0.0f, 1.0f),
                mainBoneDependency
            };

            shapeVertices[bottomDownOffset + i] =
            {
                glm::vec3(x, y, 0.0f),
                glm::vec2((x + 3) / 4, (y + 1) / 4),
                glm::vec3(0.0f, 0.0f, -1.0f),
                mainBoneDependency
            };

            unsigned int topIndex0 = topMidOffset;
            unsigned int topIndex1 = topUpOffset + i;
            unsigned int topIndex2 = topUpOffset + i + 1;
            if (topIndex2 == bottomMidOffset)
            {
                topIndex2 = topUpOffset;
            }

            indices[topIndicesOffset + i * 3] = topIndex0;
            indices[topIndicesOffset + i * 3 + 1] = topIndex1;
            indices[topIndicesOffset + i * 3 + 2] = topIndex2;

            unsigned int bottomIndex0 = bottomMidOffset;
            unsigned int bottomIndex1 = bottomDownOffset + i + 1;
            unsigned int bottomIndex2 = bottomDownOffset + i;
            if (bottomIndex1 == outOffset)
            {
                bottomIndex1 = bottomDownOffset;
            }

            indices[bottomIndicesOffset + i * 3] = bottomIndex0;
            indices[bottomIndicesOffset + i * 3 + 1] = bottomIndex1;
            indices[bottomIndicesOffset + i * 3 + 2] = bottomIndex2;
        }

        shapeVertices[outOffset + i * 2] =
        {
            glm::vec3(x, y, 1.0f),
            glm::vec2(u, 0.5f),
            glm::vec3(x, y, 0.0f),
            mainBoneDependency
        };

        shapeVertices[outOffset + i * 2 + 1] =
        {
            glm::vec3(x, y, 0.0f),
            glm::vec2(u, 1.0f),
            glm::vec3(x, y, 0.0f),
            mainBoneDependency
        };

        if (i < detail)
        {
            unsigned int quadTopLeft = outOffset + i * 2;
            unsigned int quadBottomLeft = outOffset + i * 2 + 1;
            unsigned int quadTopRight = outOffset + i * 2 + 2;
            unsigned int quadBottomRight = outOffset + i * 2 + 3;

            unsigned int indexOffset = outIndicesOffset + i * 6;

            indices[indexOffset] = quadTopLeft;
            indices[indexOffset + 1] = quadBottomLeft;
            indices[indexOffset + 2] = quadTopRight;
            indices[indexOffset + 3] = quadTopRight;
            indices[indexOffset + 4] = quadBottomLeft;
            indices[indexOffset + 5] = quadBottomRight;
        }
    }

    diffuseTextureName = "../Resources/Graphics/metal.dds";
}