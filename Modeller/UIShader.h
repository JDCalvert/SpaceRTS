#ifndef UI_SHADER_H
#define UI_SHADER_H

#include <Shader.h>

class UIComponent;

class UIShader : public Shader
{
public:
    void initialise();
    void renderUiComponent(UIComponent* component);

private:
    //Vertex array
    GLuint vertexArrayId;

    //Array attributes
    GLuint vertexPositionId;

    //Uniforms
    GLuint aspectRatioId;
    GLuint basePositionId;

    //Buffers
    GLuint vertexPositionBufferId;
    GLuint indicesBufferId;

    void renderUiComponent(UIComponent* component, glm::vec2 basePosition);
};

#endif