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
    basePositionId = glGetUniformLocation(programId, "basePosition");

    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &indicesBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
}

void UIShader::renderUiComponent(UIComponent* component)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);
    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);

    float aspectRatio = OpenGLContext::currentContext()->getAspectRatio();
    glUniform1f(aspectRatioId, aspectRatio);    

    renderUiComponent(component, glm::vec2(0.0f, 0.0f));
}

void UIShader::renderUiComponent(UIComponent* component, glm::vec2 basePosition)
{
    glUniform2f(basePositionId, basePosition.x, basePosition.y);

    Surface* surface = component->surface;

    int verticesSize = surface->verticesSize;
    glm::vec3* verticesPointer = surface->verticesPointer;
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    int indicesSize = surface->indicesSize;
    unsigned int* indicesPointer = surface->indicesPointer;
    bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

    unsigned int length = surface->length;
    GLenum renderMode = component->getRenderMode();
    glDrawElements(renderMode, length, GL_UNSIGNED_INT, 0);

    //Now draw the child components
    for (auto i = component->components.begin(); i != component->components.end(); i++)
    {
        UIComponent* childComponent = *i;
        renderUiComponent(childComponent, basePosition + component->getPosition());
    }
}