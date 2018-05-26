#include "UIShader.h"
#include "UIComponent.h"
#include "Surface.h"

#include "OpenGLContext.h"

void UIShader::initialise()
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../Resources/Shaders/UIShader.vert"},
        {GL_FRAGMENT_SHADER, "../Resources/Shaders/UIShader.frag"}
    };
    loadShaders(shaders, 2);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
    vertexTextureCoordinateId = glGetAttribLocation(programId, "vertexTextureCoordinate");

    aspectRatioId = glGetUniformLocation(programId, "aspectRatio");
    basePositionId = glGetUniformLocation(programId, "basePosition");
    textureMapId = glGetUniformLocation(programId, "textureMap");

    glGenBuffers(1, &vertexPositionBufferId);
    glGenBuffers(1, &vertexTextureCoordinateBufferId);
    glGenBuffers(1, &indicesBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 3);
    enableVertexAttribute(vertexTextureCoordinateId, vertexTextureCoordinateBufferId, 2);
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
    component->preRender();

    Surface& surface = component->surface;

    glm::vec2 componentPosition = component->getPosition();

    glm::vec2 absolutePosition = componentPosition + basePosition;
    glUniform2f(basePositionId, absolutePosition.x, absolutePosition.y);

    int verticesSize = surface.verticesSize;
    glm::vec3* verticesPointer = surface.verticesPointer;
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    int textureCoordinatesSize = surface.textureCoordinatesSize;
    glm::vec2* textureCoordinatesPointer = surface.textureCoordinatesPointer;
    bindArrayBufferData(vertexTextureCoordinateBufferId, textureCoordinatesSize, textureCoordinatesPointer);

    int indicesSize = surface.indicesSize;
    unsigned int* indicesPointer = surface.indicesPointer;
    bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, surface.diffuseMap);
    glUniform1i(textureMapId, 0);

    unsigned int length = surface.length;
    GLenum renderMode = component->getRenderMode();
    glDrawElements(renderMode, length, GL_UNSIGNED_INT, 0);

    //Now draw the child components
    for (auto i = component->components.begin(); i != component->components.end(); i++)
    {
        UIComponent* childComponent = *i;
        renderUiComponent(childComponent, basePosition + componentPosition);
    }
}