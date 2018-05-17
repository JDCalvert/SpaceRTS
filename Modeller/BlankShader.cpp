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
    colourId = glGetUniformLocation(programId, "colour");

    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &indexBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
}

void BlankShader::renderCommon(glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
    glUniform4fv(colourId, 1, &colour[0]);

    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);
    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);
}

void BlankShader::renderTriangles(Surface* surface, glm::mat4 modelViewProjectionMatrix, std::vector<unsigned int>& indices, glm::vec4 colour)
{
    if (indices.size() == 0) return;

    renderCommon(modelViewProjectionMatrix, colour);

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

void BlankShader::renderLines(Surface* surface, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    renderCommon(modelViewProjectionMatrix, colour);

    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    //Load the vertex data into memory
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    int length = surface->length;
    int indicesSize = surface->indicesSize;
    unsigned int* indicesPointer = surface->indicesPointer;

    bindElementArrayBufferData(indexBufferId, indicesSize, indicesPointer);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, (void*)0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BlankShader::renderLines(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    if (indices.size() == 0) return;

    renderCommon(modelViewProjectionMatrix, colour);

    int verticesSize = vertices.size() * sizeof(glm::vec3);
    glm::vec3* verticesPointer = &vertices[0];
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    int length = indices.size();
    int indicesSize = length * sizeof(unsigned int);
    unsigned int* indicesPointer = &indices[0];
    bindElementArrayBufferData(indexBufferId, indicesSize, indicesPointer);

    glDrawElements(GL_LINES, length, GL_UNSIGNED_INT, (void*)0);
}

void BlankShader::renderVertices(Surface* surface, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    int numVertices = surface->getVertices().size();
    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    renderVertices(numVertices, verticesSize, verticesPointer, 5, modelViewProjectionMatrix, colour);
}

void BlankShader::renderVertices(std::vector<glm::vec3>& vertices, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    if (vertices.size() == 0) return;

    int numVertices = vertices.size();
    int verticesSize = numVertices * sizeof(glm::vec3);
    glm::vec3* verticesPointer = &vertices[0];

    renderVertices(numVertices, verticesSize, verticesPointer, 10, modelViewProjectionMatrix, colour);
}

void BlankShader::renderVertices(int numVertices, int verticesSize, glm::vec3* verticesPointer, int pointSize, glm::mat4 mvp, glm::vec4 colour)
{
    renderCommon(mvp, colour);

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    glPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, numVertices);
}

void BlankShader::renderBones(Surface* surface, glm::mat4 modelViewProjectionMatrix)
{
    std::vector<BindBone>& bones = surface->getBones();

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> lineIndices;
    for (unsigned int i=0; i<bones.size(); i++)
    {
        BindBone& bone = bones[i];
        vertices.push_back(bone.absolute[3]);

        if (bone.parent != -1)
        {
            lineIndices.push_back(bone.parent);
            lineIndices.push_back(i);
        }
    }

    renderVertices(vertices, modelViewProjectionMatrix, glm::vec4(1.0f));
    renderLines(vertices, lineIndices, modelViewProjectionMatrix, glm::vec4(1.0f));
}