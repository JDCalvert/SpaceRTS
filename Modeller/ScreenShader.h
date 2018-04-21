#ifndef SCREEN_SHADER_H
#define SCREEN_SHADER_H

#include "Shader.h"

class Renderer;

class ScreenShader: public Shader
{
public:
    ScreenShader(Renderer* renderer);

    void initialise();
    void render();

private:
    GLuint vertexPositionId;

    GLuint textureId;

    GLuint vertexBufferId;
};

#endif