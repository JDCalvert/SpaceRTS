#include "SimpleShader.h"
#include "Renderer.h"
#include "Surface.h"

SimpleShader::SimpleShader(Renderer* renderer): Shader(renderer)
{
}

void SimpleShader::initialise()
{
    //Load up the shaders and link them into a program
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "SimpleShader.vert"},
        {GL_FRAGMENT_SHADER, "SimpleShader.frag"}
    };
    programId = Shader::loadShaders(shaders, 2);

    //Get the identifiers for the shader variables
    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    vertexTextureCoordinateId = glGetAttribLocation(programId, "vertexTextureCoordinate");

    diffuseMapId = glGetUniformLocation(programId, "diffuseTexture");

    modelViewProjectionMatrixId = glGetUniformLocation(programId, "modelViewProjectionMatrix");

    //Generate data buffers
    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &vertexTextureCoordinateBufferId);
    glGenBuffers(1, &indicesBufferId);
}

void SimpleShader::renderSurface(Surface* surface, glm::mat4 modelMatrix)
{
    glUseProgram(programId);

    int verticesSize = surface->verticesSize;
    int textureCoordinatesSize = surface->textureCoordinatesSize;
    int indicesSize = surface->indicesSize;

    glm::vec3* verticesPointer = surface->verticesPointer;
    glm::vec2* textureCoordinatesPointer = surface->textureCoordinatesPointer;
    unsigned int* indicesPointer = surface->indicesPointer;

    int length = surface->length;

    //Load the data into graphics memory
    renderer->bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);
    renderer->bindArrayBufferData(vertexTextureCoordinateBufferId, textureCoordinatesSize, textureCoordinatesPointer);

    renderer->bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

    //Link the data to the shader variables
    renderer->enableVertexAttrib(vertexPositionId, vertexPositionBufferId, 3);
    renderer->enableVertexAttrib(vertexTextureCoordinateId, vertexTextureCoordinateBufferId, 2);

    //Textures
    GLuint diffuseMap = surface->diffuseMap;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glUniform1i(diffuseMapId, 0);

    //Matrices
    glm::mat4 modelViewMatrix = renderer->viewMatrix * modelMatrix;
    glm::mat4 modelViewProjectionMatrix = renderer->projectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(vertexPositionId);
    glDisableVertexAttribArray(vertexTextureCoordinateId);
}