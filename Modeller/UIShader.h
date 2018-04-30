#ifndef UI_SHADER_H
#define UI_SHADER_H

#include <Shader.h>

class UIPanel;

class UIShader : public Shader
{
public:
    void initialise();

    void renderUiPanel(UIPanel* panel);

private:
    //Vertex array
    GLuint vertexArrayId;

    //Array attributes
    GLuint vertexPositionId;

    //Buffers
    GLuint vertexPositionBufferId;
};

#endif