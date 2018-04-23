#ifndef SCREEN_SHADER_H
#define SCREEN_SHADER_H

#include "Shader.h"

class ScreenShader : public Shader
{
public:

    void initialise();
    void render(GLuint texture);

private:

    //Out vertex array
    GLuint vertexArrayId;

    //Handle for the vertex position attribute
    GLuint vertexPositionId;

    //Handle for the texture to be rendered to the panel
    GLuint textureId;

    //Buffer containing the vertex data
    GLuint vertexBufferId;
};

#endif