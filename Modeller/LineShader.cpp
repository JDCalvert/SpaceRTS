#include "LineShader.h"

#include <set>

#include <Surface.h>

void LineShader::initialise()
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "LineShader.vert"},
        {GL_FRAGMENT_SHADER, "LineShader.frag"}
    };
    programId = loadShaders(shaders, 2);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    modelViewProjectionMatrixId = glGetUniformLocation(programId, "modelViewProjectionMatrix");

    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &indexBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
}

void LineShader::renderSurface(Surface* surface, glm::mat4 modelViewProjectionMatrix)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;

    //Load the vertex data into memory
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

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

    int size = lineIndices.size() * sizeof(unsigned int);
    bindElementArrayBufferData(indexBufferId, size, &lineIndices[0]);

    //glEnable(GL_LINE_SMOOTH);
    glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, (void*)0);
}