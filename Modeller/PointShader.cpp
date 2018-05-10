#include "PointShader.h"

#include <OpenGLContext.h>
#include <Surface.h>

void PointShader::initialise()
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "PointShader.vert"},
        {GL_FRAGMENT_SHADER, "PointShader.frag"}
    };
    loadShaders(shaders, 2);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    modelViewProjectionMatrixId = glGetUniformLocation(programId, "modelViewProjectionMatrix");

    glGenBuffers(1, &vertexPositionBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
}

void PointShader::renderSurface(Surface* surface, glm::mat4 modelViewProjectionMatrix)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_FALSE);
    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);

    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, surface->getVertices().size());
}