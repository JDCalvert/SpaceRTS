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

private:
    GLuint vertexArrayId;

    GLuint vertexPositionId;
    GLuint modelViewProjectionMatrixId;

    GLuint vertexPositionBufferId;
};

#endif