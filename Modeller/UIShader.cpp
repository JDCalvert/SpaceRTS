#include "UIShader.h"
#include "UIComponent.h"
#include "Surface.h"

#include "OpenGLContext.h"

void UIShader::initialise()
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "UIShader.vert"},
        {GL_FRAGMENT_SHADER, "UIShader.frag"}
    };
    programId = loadShaders(shaders, 2);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");

    aspectRatioId = glGetUniformLocation(programId, "aspectRatio");
    //basePositionId = glGetUniformLocation(programId, "basePosition");

    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &indicesBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
}

void UIShader::renderUiComponent(UIComponent* component)
{
    renderUiComponent(component, glm::vec2(0.0f, 0.0f));
}

void UIShader::renderUiComponent(UIComponent* component, glm::vec2 basePosition)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    Surface* surface = component->surface;

    int verticesSize = surface->verticesSize;
    int indicesSize = surface->indicesSize;

    glm::vec3* verticesPointer = surface->verticesPointer;
    unsigned int* indicesPointer = surface->indicesPointer;

    unsigned int length = surface->length;

    bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

    float aspectRatio = OpenGLContext::currentContext()->getAspectRatio();
    glUniform1f(aspectRatioId, aspectRatio);
    //glUniform2fv(basePositionId, 1, &basePosition[0]);

    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);
    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);

    GLenum renderMode = component->getRenderMode();
    glDrawElements(GL_TRIANGLE_STRIP, length, GL_UNSIGNED_INT, 0);

    //Now draw the 
    for (auto i = component->components.begin(); i != component->components.end(); i++)
    {
        UIComponent* childComponent = *i;
        renderUiComponent(childComponent, basePosition + component->getPosition());
    }
}