#include "ScreenShader.h"
#include "Renderer.h"

ScreenShader::ScreenShader(Renderer* renderer): Shader(renderer)
{
}

void ScreenShader::initialise()
{
    ShaderInfo shaders[]
    {
        {GL_VERTEX_SHADER, "ScreenShader.vert"},
    {GL_FRAGMENT_SHADER, "ScreenShader.frag"}
    };
    programId = Shader::loadShaders(shaders, 2);

    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    textureId = glGetUniformLocation(programId, "renderedTexture");

    GLfloat screenCoords[] =
    {
        -1.0f, 1.0f, //Top left
        -1.0f,-1.0f, //Bottom left
        1.0f, 1.0f, //Top right
        1.0f,-1.0f  //Bottom right
    };


    glGenBuffers(1, &vertexBufferId);
    renderer->bindArrayBufferData(vertexBufferId, sizeof(screenCoords), &screenCoords[0]);
}