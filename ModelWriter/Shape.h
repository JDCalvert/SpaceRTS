#ifndef SHAPE_H
#define SHAPE_H

#include <string>

#include <Surface.h>

struct Vertex
{
    glm::vec3 vertex;
    glm::vec2 textureCoordinate;
    glm::vec3 normal;
    glm::vec4 boneIndicesAndWeights;
};

class Shape : public Surface
{
public:
    Shape();

    virtual void buildAndWrite() final;

protected:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Bone> bones;

    glm::vec4 mainBoneDependency;

    virtual void build() = 0;
    virtual std::string getFileName() = 0;

    void addTriangle(glm::vec3 vertices[3], glm::vec2 textureCoordinates[3], glm::vec4 boneDependency);
    void addTriangle(glm::vec3 vertexPosition0, glm::vec3 vertexPosition1, glm::vec3 vertexPosition2,
                     glm::vec2 textureCoordinate0, glm::vec2 textureCoordinate1, glm::vec2 textureCoordinate2,
                     glm::vec4 boneDependency);
    void addQuad(glm::vec3 topLeft, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 topRight,
                 glm::vec2 topLeftTexCoord, glm::vec2 bottomLeftTexCoord, glm::vec2 bottomRightTexCoord, glm::vec2 topRightTexCoord,
                 glm::vec3 normal, glm::vec4 boneDependency);

    
};

#endif