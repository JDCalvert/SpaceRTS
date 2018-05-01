#ifndef UI_SHADER_H
#define UI_SHADER_H

#include <Shader.h>

class UIComponent;

class UIShader : public Shader
{
public:
    void initialise();

    void renderUiComponent(UIComponent* panel);

private:
    //Vertex array
    GLuint vertexArrayId;

    //Array attributes
    GLuint vertexPositionId;

    //Uniforms
    GLuint aspectRatioId;

    //Buffers
    GLuint vertexPositionBufferId;
};

#endif