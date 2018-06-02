#include "CapitalShip.h"

std::string CapitalShip::getFileName()
{
    return "capital-ship";
}

void CapitalShip::build()
{
    glm::vec3 frontMiddleTop    ( 0.000f,  8.000f,  0.050f);
    glm::vec3 backMiddleTop     ( 0.000f, -8.000f,  1.300f);
    glm::vec3 backLeftTop       (-4.400f, -6.000f,  0.050f);
    glm::vec3 backRightTop      ( 4.400f, -6.000f,  0.050f);
    glm::vec3 frontMiddleBottom ( 0.000f,  8.000f, -0.050f);
    glm::vec3 backMiddleBottom  ( 0.000f, -8.000f, -1.200f);
    glm::vec3 backRightBottom   ( 4.400f, -6.000f, -0.050f);
    glm::vec3 backLeftBottom    (-4.400f, -6.000f, -0.050f);

    glm::vec2 frontMiddleTexCoord = glm::vec2(0.5, 0.0);
    glm::vec2 frontRightTexCoord = glm::vec2(1.0, 0.0);
    glm::vec2 frontLeftTexCoord = glm::vec2(0.0, 0.0);
    glm::vec2 backLeftTexCoord = glm::vec2(0.0, 1.0);
    glm::vec2 backRightTexCoord = glm::vec2(1.0, 1.0);
    glm::vec2 backMiddleTexCoord = glm::vec2(0.5, 1.0);

    glm::vec3 leftNormal = glm::normalize(glm::cross(frontMiddleBottom - frontMiddleTop, backMiddleTop - frontMiddleTop));
    glm::vec3 rightNormal = glm::normalize(glm::cross(backRightBottom - backRightTop, frontMiddleTop - backRightTop));
    glm::vec3 backNormal = glm::vec3(0.000f, -1.000f, 0.000f);

    addTriangle(frontMiddleTop, backLeftTop, backMiddleTop, frontMiddleTexCoord, backLeftTexCoord, backMiddleTexCoord, mainBoneDependency);
    addTriangle(frontMiddleTop, backMiddleTop, backRightTop, frontMiddleTexCoord, backMiddleTexCoord, backRightTexCoord, mainBoneDependency);
    addTriangle(frontMiddleBottom, backMiddleBottom, backLeftBottom, frontMiddleTexCoord, backMiddleTexCoord, backLeftTexCoord, mainBoneDependency);
    addTriangle(frontMiddleBottom, backRightBottom, backMiddleBottom, frontMiddleTexCoord, backRightTexCoord, backMiddleTexCoord, mainBoneDependency);
    addTriangle(backMiddleTop, backLeftTop, backRightTop, frontMiddleTexCoord, backLeftTexCoord, backRightTexCoord, mainBoneDependency);
    addTriangle(backMiddleBottom, backRightBottom, backLeftBottom, backMiddleTexCoord, frontRightTexCoord, frontLeftTexCoord, mainBoneDependency);

    addQuad(frontMiddleTop, frontMiddleBottom, backLeftTop, backLeftBottom, frontRightTexCoord, frontLeftTexCoord, backRightTexCoord, backLeftTexCoord, leftNormal, mainBoneDependency);
    addQuad(backRightTop, backRightBottom, frontMiddleTop, frontMiddleBottom, backLeftTexCoord, backRightTexCoord, frontLeftTexCoord, frontRightTexCoord, rightNormal, mainBoneDependency);
    addQuad(backLeftTop, backLeftBottom, backRightTop, backRightBottom, glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0), glm::vec2(176, 0.0), glm::vec2(176, 1.0), backNormal, mainBoneDependency);

    diffuseTextureName = "../Resources/Graphics/metal.dds";
}