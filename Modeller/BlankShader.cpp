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

void BlankShader::renderLines(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour, GLfloat lineWidth)
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

    OpenGLContext::currentContext()->setLineWidth(lineWidth);
    glDrawElements(GL_LINES, length, GL_UNSIGNED_INT, (void*)0);
}

void BlankShader::renderVertices(Surface* surface, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    int numVertices = surface->vertices.size();
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

void BlankShader::renderVertices(int numVertices, int verticesSize, glm::vec3* verticesPointer, GLfloat pointSize, glm::mat4 mvp, glm::vec4 colour)
{
    renderCommon(mvp, colour);

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    OpenGLContext::currentContext()->setPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, numVertices);
}

void BlankShader::renderTextureCoordinateVertices(Surface* surface, std::vector<unsigned int>& indices, glm::vec4 colour)
{
    std::vector<glm::vec2>& textureCoordinates = surface->textureCoordinates;
    glm::vec2 minus1(-1.0f, -1.0f);

    std::vector<glm::vec3> vertices;
    for (unsigned int i=0; i<indices.size(); i++)
    {
        unsigned int index = indices[i];
        vertices.push_back(glm::vec3(textureCoordinates[index] * 2.0f + minus1, 0.0f));
    }

    renderVertices(vertices, glm::mat4(), colour);
}

void BlankShader::renderTextureCoordinateLines(Surface* surface, std::vector<unsigned int>& indices, glm::vec4 colour)
{
    std::vector<glm::vec2>& textureCoordinates = surface->textureCoordinates;

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> newLineIndices;
    for (unsigned int i=0; i<indices.size(); i+=3)
    {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i+1];
        unsigned int index2 = indices[i+2];

        int baseIndex = vertices.size();
        glm::vec2 minus1(-1.0f, -1.0f);

        vertices.push_back(glm::vec3(textureCoordinates[index0] * 2.0f + minus1, 0.0f));
        vertices.push_back(glm::vec3(textureCoordinates[index1] * 2.0f + minus1, 0.0f));
        vertices.push_back(glm::vec3(textureCoordinates[index2] * 2.0f + minus1, 0.0f));

        newLineIndices.push_back(baseIndex);
        newLineIndices.push_back(baseIndex + 1);
        newLineIndices.push_back(baseIndex + 1);
        newLineIndices.push_back(baseIndex + 2);
        newLineIndices.push_back(baseIndex + 2);
        newLineIndices.push_back(baseIndex);
    }

    renderLines(vertices, newLineIndices, glm::mat4(), colour, 2);
}

void BlankShader::renderVerticesWithAxes(Surface* surface, std::vector<unsigned int>& indices, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    std::vector<glm::vec3> verticesToRender;
    std::vector<glm::vec3> normalLineVertices;
    std::vector<glm::vec3> tangentLineVertices;
    std::vector<glm::vec3> bitangentLineVertices;
    std::vector<unsigned int> normalLineIndices;

    for (unsigned int i = 0; i<indices.size(); i++)
    {
        unsigned int index = indices[i];

        glm::vec3 vertexPosition = surface->vertices[index];
        verticesToRender.push_back(vertexPosition);

        normalLineVertices.push_back(vertexPosition);
        normalLineVertices.push_back(vertexPosition + surface->normals[index]);

        tangentLineVertices.push_back(vertexPosition);
        tangentLineVertices.push_back(vertexPosition + surface->tangents[index]);

        bitangentLineVertices.push_back(vertexPosition);
        bitangentLineVertices.push_back(vertexPosition + surface->bitangents[index]);

        normalLineIndices.push_back(i * 2);
        normalLineIndices.push_back(i * 2 + 1);
    }

    renderVertices(verticesToRender, modelViewProjectionMatrix, colour);
    renderLines(tangentLineVertices, normalLineIndices, modelViewProjectionMatrix, glm::vec4(colour.r, 0.0f, 0.0f, colour.a), 2);
    renderLines(bitangentLineVertices, normalLineIndices, modelViewProjectionMatrix, glm::vec4(0.0f, colour.g, 0.0f, colour.a), 2);
    renderLines(normalLineVertices, normalLineIndices, modelViewProjectionMatrix, glm::vec4(0.0f, 0.0f, colour.b, colour.a), 2);
}

void BlankShader::renderBones(Surface* surface, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> lineIndices;

    for (unsigned int i = 0; i<surface->bones.size(); i++)
    {
        Bone& bone = surface->bones[i];

        vertices.push_back(bone.absolute[3]);
        if (bone.parent != -1)
        {
            lineIndices.push_back(bone.parent);
            lineIndices.push_back(i);
        }
    }

    renderVertices(vertices, modelViewProjectionMatrix, colour);
    renderLines(vertices, lineIndices, modelViewProjectionMatrix, colour, 2);
}

void BlankShader::renderBonesWithAxes(Surface* surface, std::vector<unsigned int>& indices, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour)
{
    std::vector<Bone>& bones = surface->bones;

    std::vector<glm::vec3> xLineVertices;
    std::vector<glm::vec3> yLineVertices;
    std::vector<glm::vec3> zLineVertices;
    std::vector<unsigned int> newLineIndices;
    for (unsigned int index : indices)
    {
        Bone& bone = bones[index];

        glm::vec4 oPosition = bone.absolute * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 xPosition = bone.absolute * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 yPosition = bone.absolute * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        glm::vec4 zPosition = bone.absolute * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

        xLineVertices.push_back(oPosition);
        xLineVertices.push_back(xPosition);

        yLineVertices.push_back(oPosition);
        yLineVertices.push_back(yPosition);

        zLineVertices.push_back(oPosition);
        zLineVertices.push_back(zPosition);

        unsigned int baseIndex = newLineIndices.size();
        newLineIndices.push_back(baseIndex);
        newLineIndices.push_back(baseIndex + 1);
    }

    renderLines(xLineVertices, newLineIndices, modelViewProjectionMatrix, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 2);
    renderLines(yLineVertices, newLineIndices, modelViewProjectionMatrix, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 2);
    renderLines(zLineVertices, newLineIndices, modelViewProjectionMatrix, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 2);
}