#ifndef POINT_SHADER_H
#define POINT_SHADER_H

#include <glm\glm.hpp>

#include <Shader.h>

class Surface;

class PointShader : public Shader
{
public:
    void initialise();
    
    void renderSurface(Surface* surface, glm::mat4 modelViewProjectionMatrix);
    void renderPoints(std::vector<glm::vec3>& points, glm::mat4 modelViewProjectionMatrix);

    void render(int numVertices, int verticesSize, glm::vec3* verticesPointer, int pointSize, glm::mat4 mvp);

private:
    GLuint vertexArrayId;

    GLuint vertexPositionId;
    GLuint modelViewProjectionMatrixId;

    GLuint vertexPositionBufferId;
};

#endif