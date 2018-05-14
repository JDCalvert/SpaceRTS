#ifndef UI_SHADER_H
#define UI_SHADER_H

#include "Shader.h"

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
    GLuint vertexTextureCoordinateId;

    //Uniforms
    GLuint aspectRatioId;
    GLuint basePositionId;
    GLuint textureMapId;

    //Buffers
    GLuint vertexPositionBufferId;
    GLuint vertexTextureCoordinateBufferId;
    GLuint indicesBufferId;

    void renderUiComponent(UIComponent* component, glm::vec2 basePosition);
};

#endif