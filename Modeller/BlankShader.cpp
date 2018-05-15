#include "BlankShader.h"

#include <set>

#include <OpenGLContext.h>
#include <Surface.h>

void BlankShader::initialise()
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "BlankShader.vert"},
        {GL_FRAGMENT_SHADER, "BlankShader.frag"}
    };
    loadShaders(shaders, 2);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    modelViewProjectionMatrixId = glGetUniformLocation(programId, "modelViewProjectionMatrix");

    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &indexBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
}

void BlankShader::renderCommon(glm::mat4 modelViewProjectionMatrix)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);
    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);
}

void BlankShader::renderTriangles(Surface* surface, glm::mat4 modelViewProjectionMatrix, std::vector<unsigned int>& indices)
{
    if (indices.size() == 0) return;

    renderCommon(modelViewProjectionMatrix);

    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    //Load the vertex data into memory
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    int numIndices = indices.size();
    int indicesSize = numIndices * sizeof(unsigned int);
    unsigned int* indicesPointer = &indices[0];

    bindElementArrayBufferData(indexBufferId, indicesSize, indicesPointer);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
}

void BlankShader::renderLines(Surface* surface, glm::mat4 modelViewProjectionMatrix)
{
    renderCommon(modelViewProjectionMatrix);

    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    //Load the vertex data into memory
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    //We want to draw lines, so we need to figure out where those lines are
    //Go through and find each pair of vertices that are in a triangle and
    //make an index for them.
    std::vector<unsigned int> indices = surface->getIndices();
    std::set<std::pair<unsigned int, unsigned int>> lines;
    for (auto i=indices.begin(); i != indices.end(); i+=3)
    {
        unsigned int index1 = *(i);
        unsigned int index2 = *(i+1);
        unsigned int index3 = *(i+2);

        lines.insert(std::pair<unsigned int, unsigned int>(index1, index2));
        lines.insert(std::pair<unsigned int, unsigned int>(index1, index3));
        lines.insert(std::pair<unsigned int, unsigned int>(index2, index3));
    }

    std::vector<unsigned int> lineIndices;
    for (auto i=lines.begin(); i != lines.end(); i++)
    {
        auto pair = *i;
        
        lineIndices.push_back(pair.first);
        lineIndices.push_back(pair.second);
    }

    int numIndices = lineIndices.size();
    int indicesSize = numIndices * sizeof(unsigned int);
    unsigned int* indicesPointer = &lineIndices[0];

    bindElementArrayBufferData(indexBufferId, indicesSize, indicesPointer);

    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, (void*)0);
}

void BlankShader::renderVertices(Surface* surface, glm::mat4 modelViewProjectionMatrix)
{
    int numVertices = surface->getVertices().size();
    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    renderVertices(numVertices, verticesSize, verticesPointer, 5, modelViewProjectionMatrix);
}

void BlankShader::renderVertices(std::vector<glm::vec3>& vertices, glm::mat4 modelViewProjectionMatrix)
{
    if (vertices.size() == 0) return;

    int numVertices = vertices.size();
    int verticesSize = numVertices * sizeof(glm::vec3);
    glm::vec3* verticesPointer = &vertices[0];

    renderVertices(numVertices, verticesSize, verticesPointer, 20, modelViewProjectionMatrix);
}

void BlankShader::renderVertices(int numVertices, int verticesSize, glm::vec3* verticesPointer, int pointSize, glm::mat4 mvp)
{
    renderCommon(mvp);

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    glPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, numVertices);
}