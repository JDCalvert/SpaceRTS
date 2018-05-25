#include "ScreenShader.h"
#include "OpenGLContext.h"

void ScreenShader::initialise()
{
    //Create the program
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../Resources/Shaders/ScreenShader.vert"},
        {GL_FRAGMENT_SHADER, "../Resources/Shaders/ScreenShader.frag"}
    };
    loadShaders(shaders, 2);

    //Get the program variables
    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    textureId = glGetUniformLocation(programId, "renderedTexture");

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

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

    //Enable the vertexPosition attribute, bind our new buffer to it, and tell OpenGL what the data is
    glEnableVertexAttribArray(vertexPositionId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glVertexAttribPointer(vertexPositionId, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void ScreenShader::render(GLuint renderedTexture)
{
    //Enable this shader's program and vertex array
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    //Bind renderedTexture to the first texture location
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glUniform1i(textureId, 0);

    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_FALSE);

    //Draw the vertices as a triangle strip, so we only have to pass them up once
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}