#include "UIShader.h"
#include "UIPanel.h"

#include "OpenGLContext.h"

UIShader::UIShader(OpenGLContext* glContext) : Shader(glContext)
{
}

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

    glGenBuffers(1, &vertexPositionBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 2);
}

void UIShader::renderUiPanel(UIPanel* panel)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    std::vector<glm::vec2> vertices = 
    {
        glm::vec2(panel->position.x, panel->position.y),
        glm::vec2(panel->position.x, panel->position.y + panel->size.x),
        glm::vec2(panel->position.x + panel->size.x, panel->position.y),
        glm::vec2(panel->position.x + panel->size.x, panel->position.y + panel->size.y)
    };

    int length = vertices.size();
    int verticesSize = length * sizeof(glm::vec2);
    glm::vec2* verticesPointer = &vertices[0];

    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    glContext->setEnabled(GL_DEPTH_TEST, GL_FALSE);
    glContext->setEnabled(GL_BLEND, GL_TRUE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
}