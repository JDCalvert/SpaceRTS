#include "Cube.h"

std::string Cube::getFileName()
{
    return "cube";
}

void Cube::build()
{
    glm::vec3 leftFrontTop(    -1.0f,  1.0f,  1.0f);
    glm::vec3 leftBackTop(     -1.0f, -1.0f,  1.0f);
    glm::vec3 rightBackTop(     1.0f, -1.0f,  1.0f);
    glm::vec3 rightFrontTop(    1.0f,  1.0f,  1.0f);

    glm::vec3 leftFrontBottom( -1.0f,  1.0f, -1.0f);
    glm::vec3 leftBackBottom(  -1.0f, -1.0f, -1.0f);
    glm::vec3 rightBackBottom(  1.0f, -1.0f, -1.0f);
    glm::vec3 rightFrontBottom( 1.0f,  1.0f, -1.0f);

    glm::vec2 topLeftTexCoord(      0.0f, 0.0f);
    glm::vec2 bottomLeftTexCoord(   0.0f, 1.0f);
    glm::vec2 bottomRightTexCoord(  1.0f, 1.0f);
    glm::vec2 topRightTexCoord(     1.0, 0.0);

    glm::vec3 normalBottom( 0.0f,  0.0f, -1.0f);
    glm::vec3 normalTop(    0.0f,  0.0f,  1.0f);
    glm::vec3 normalLeft(  -1.0f,  0.0f,  0.0f);
    glm::vec3 normalFront(  0.0f,  1.0f,  0.0f);
    glm::vec3 normalRight(  1.0f,  0.0f,  0.0f);
    glm::vec3 normalBack(   0.0f, -1.0f, -0.0f);

    addQuad(rightFrontTop, leftFrontTop, leftBackTop, rightBackTop, topRightTexCoord, topLeftTexCoord, bottomLeftTexCoord, bottomRightTexCoord, normalTop, mainBoneDependency);
    addQuad(leftBackBottom, leftFrontBottom, rightFrontBottom, rightBackBottom, topLeftTexCoord, bottomLeftTexCoord, bottomRightTexCoord, topRightTexCoord, normalBottom, mainBoneDependency);
    addQuad(leftFrontTop, leftFrontBottom, leftBackBottom, leftBackTop, topLeftTexCoord, bottomLeftTexCoord, bottomRightTexCoord, topRightTexCoord, normalLeft, mainBoneDependency);
    addQuad(rightFrontTop, rightBackTop, rightBackBottom, rightFrontBottom, topRightTexCoord, topLeftTexCoord, bottomLeftTexCoord, bottomRightTexCoord, normalRight, mainBoneDependency);
    addQuad(rightFrontTop, rightFrontBottom, leftFrontBottom, leftFrontTop, topLeftTexCoord, bottomLeftTexCoord, bottomRightTexCoord, topRightTexCoord, normalFront, mainBoneDependency);
    addQuad(rightBackTop, leftBackTop, leftBackBottom, rightBackBottom, topRightTexCoord, topLeftTexCoord, bottomLeftTexCoord, bottomRightTexCoord, normalBack, mainBoneDependency);

    diffuseTextureName = "../Resources/Graphics/metal.dds";
}