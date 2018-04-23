#include "ScreenShader.h"

void ScreenShader::initialise()
{
    //Create the program
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "ScreenShader.vert"},
        {GL_FRAGMENT_SHADER, "ScreenShader.frag"}
    };
    programId = loadShaders(shaders, 2);

    //Get the program variables
    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    textureId = glGetUniformLocation(programId, "renderedTexture");

    //Specify our screen coordinates
    static const GLfloat screenCoords[] =
    {
        -1.0f, 1.0f, //Top left
        -1.0f,-1.0f, //Bottom left
         1.0f, 1.0f, //Top right
         1.0f,-1.0f  //Bottom right
    };

    //Generate a buffer and fill it with the screen coordinates
    glGenBuffers(1, &vertexBufferId);
    bindArrayBufferData(vertexBufferId, sizeof(screenCoords), (void*)&screenCoords[0]);
}

void ScreenShader::render(GLuint renderedTexture)
{
    glUseProgram(programId);

    //Bind renderedTexture to the first texture location
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glUniform1i(textureId, 0);

    glEnableVertexAttribArray(vertexPositionId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glVertexAttribPointer(vertexPositionId, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(vertexPositionId);
}