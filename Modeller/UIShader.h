#ifndef UI_SHADER_H
#define UI_SHADER_H

#include <Shader.h>

class UIComponent;

class UIShader : public Shader
{
public:
    void initialise();

    void renderUiComponent(UIComponent* panel);
    void buildVertices(UIComponent* component, glm::vec2 basePosition, std::vector<glm::vec2> &vertices, std::vector<unsigned int> &indices);

private:
    //Vertex array
    GLuint vertexArrayId;

    //Array attributes
    GLuint vertexPositionId;

    //Uniforms
    GLuint aspectRatioId;

    //Buffers
    GLuint vertexPositionBufferId;
    GLuint indicesBufferId;
};

#endif