#include "UIShader.h"
#include "UIComponent.h"

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

    glGenBuffers(1, &vertexPositionBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 2);
}

void UIShader::renderUiComponent(UIComponent* panel)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    std::vector<glm::vec2> vertices = 
    {
        glm::vec2(panel->position.x, panel->position.y),
        glm::vec2(panel->position.x, panel->position.y + panel->size.y),
        glm::vec2(panel->position.x + panel->size.x, panel->position.y),
        glm::vec2(panel->position.x + panel->size.x, panel->position.y + panel->size.y)
    };

    int length = vertices.size();
    int verticesSize = length * sizeof(glm::vec2);
    glm::vec2* verticesPointer = &vertices[0];

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    float aspectRatio = OpenGLContext::currentContext()->getAspectRatio();
    glUniform1f(aspectRatioId, aspectRatio);

    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);
    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
}