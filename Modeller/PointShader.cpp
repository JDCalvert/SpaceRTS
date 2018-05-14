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
    int numVertices = surface->getVertices().size();
    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    render(numVertices, verticesSize, verticesPointer, 5, modelViewProjectionMatrix);
}

void PointShader::renderPoints(std::vector<glm::vec3>& points, glm::mat4 modelViewProjectionMatrix)
{
    int numVertices = points.size();
    
    if (numVertices == 0) return;

    int verticesSize = numVertices * sizeof(glm::vec3);
    glm::vec3* verticesPointer = &points[0];

    render(numVertices, verticesSize, verticesPointer, 10, modelViewProjectionMatrix);
}

void PointShader::render(int numVertices, int verticesSize, glm::vec3* verticesPointer, int pointSize, glm::mat4 mvp)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &mvp[0][0]);

    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_FALSE);
    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);

    glPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, numVertices);
}