#ifndef LINE_SHADER_H
#define LINE_SHADER_H

#include <glm\glm.hpp>
#include <Shader.h>

class Surface;

class LineShader : public Shader
{
public:
    void initialise();
    void renderSurface(Surface* surface, glm::mat4 modelViewProjectionMatrix);

private:
    GLuint vertexArrayId;

    GLuint vertexPositionId;
    GLuint modelViewProjectionMatrixId;

    GLuint vertexPositionBufferId;
    GLuint indexBufferId;
};

#endif LINE_SHADER_H