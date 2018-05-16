#ifndef LINE_SHADER_H
#define LINE_SHADER_H

#include <glm\glm.hpp>
#include <Shader.h>

class Surface;

class BlankShader : public Shader
{
public:
    void initialise();
    
    void renderCommon(glm::mat4 modelViewProjectionMatrix, glm::vec4 colour);

    void renderVertices(Surface* surface, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour);
    void renderVertices(std::vector<glm::vec3>& vertices, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour);
    void renderVertices(int numVertices, int verticesSize, glm::vec3* verticesPointer, int pointSize, glm::mat4 mvp, glm::vec4 colour);

    void renderLines(Surface* surface, glm::mat4 modelViewProjectionMatrix, glm::vec4 colour);

    void renderTriangles(Surface* surface, glm::mat4 modelViewProjectionMatrix, std::vector<unsigned int>& indices, glm::vec4 colour);

    void renderBones(Surface* surface, glm::mat4 modelViewProjectionMatrix);

private:
    GLuint vertexArrayId;

    GLuint vertexPositionId;
    GLuint modelViewProjectionMatrixId;
    GLuint colourId;

    GLuint vertexPositionBufferId;
    GLuint indexBufferId;
};

#endif