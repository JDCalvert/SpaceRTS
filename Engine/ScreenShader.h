#ifndef SCREEN_SHADER_H
#define SCREEN_SHADER_H

#include "Shader.h"

/**
    Basic screen shader that simply renders the given texture to the whole screen
*/
class ScreenShader : public Shader
{
public:
    void initialise();
    void render(GLuint texture);

private:
    //Our vertex array
    GLuint vertexArrayId;

    //Handle for the vertex position attribute
    GLuint vertexPositionId;

    //Handle for the texture to be rendered to the panel
    GLuint textureId;

    //Buffer containing the vertex data
    GLuint vertexBufferId;
};

#endif