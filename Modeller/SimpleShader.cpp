#include "SimpleShader.h"
#include "OpenGLContext.h"
#include "Surface.h"

void SimpleShader::initialise()
{
    //Load up the shaders and link them into a program
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "SimpleShader.vert"},
        {GL_FRAGMENT_SHADER, "SimpleShader.frag"}
    };
    loadShaders(shaders, 2);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    //Get the identifiers for the shader variables
    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    vertexTextureCoordinateId = glGetAttribLocation(programId, "vertexTextureCoordinate");

    diffuseMapId = glGetUniformLocation(programId, "diffuseTexture");

    modelViewProjectionMatrixId = glGetUniformLocation(programId, "modelViewProjectionMatrix");

    //Generate data buffers
    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &vertexTextureCoordinateBufferId);
    glGenBuffers(1, &indicesBufferId);

    //Enable the vertexPosition and textureCoordinate attributes, binding out buffers to them
    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
    enableVertexAttribute(vertexTextureCoordinateId, vertexTextureCoordinateBufferId, 2);
}

void SimpleShader::renderSurface(Surface* surface, glm::mat4 modelViewProjectionMatrix)
{
    glUseProgram(programId);

    glBindVertexArray(vertexArrayId);

    int verticesSize = surface->verticesSize;
    int textureCoordinatesSize = surface->textureCoordinatesSize;
    int indicesSize = surface->indicesSize;

    glm::vec3* verticesPointer = surface->verticesPointer;
    glm::vec2* textureCoordinatesPointer = surface->textureCoordinatesPointer;
    unsigned int* indicesPointer = surface->indicesPointer;

    int length = surface->length;

    //Load the data into graphics memory
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);
    bindArrayBufferData(vertexTextureCoordinateBufferId, textureCoordinatesSize, textureCoordinatesPointer);

    bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

    //Textures
    GLuint diffuseMap = surface->diffuseMap;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glUniform1i(diffuseMapId, 0);

    glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);
    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_TRUE);

    //Actually draw the triangles
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, (void*)0);
}