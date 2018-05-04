#ifndef UI_SHADER_H
#define UI_SHADER_H

#include <Shader.h>

class UIComponent;

class UIShader : public Shader
{
public:
    void initialise();

    void renderUiComponent(UIComponent* component);
    void renderUiComponent(UIComponent* component, glm::vec2 basePosition);

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
};

#endif