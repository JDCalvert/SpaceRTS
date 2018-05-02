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
    glGenBuffers(1, &indicesBufferId);

    enableVertexAttribute(vertexPositionId, vertexPositionBufferId, 2);
}

void UIShader::renderUiComponent(UIComponent* component)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
    buildVertices(component, glm::vec2(0.0f, 0.0f), vertices, indices);

    int verticesSize = vertices.size() * sizeof(glm::vec2);
    glm::vec2* verticesPointer = &vertices[0];
    bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);

    int length = indices.size();
    int indicesSize = length * sizeof(unsigned int);
    unsigned int* indicesPointer = &indices[0];

    bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

    float aspectRatio = OpenGLContext::currentContext()->getAspectRatio();
    glUniform1f(aspectRatioId, aspectRatio);

    OpenGLContext::currentContext()->setEnabled(GL_DEPTH_TEST, GL_FALSE);
    OpenGLContext::currentContext()->setEnabled(GL_BLEND, GL_TRUE);

    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, 0);
}

void UIShader::buildVertices(UIComponent* component, glm::vec2 basePosition, std::vector<glm::vec2> &vertices, std::vector<unsigned int> &indices)
{
    unsigned int baseIndex = vertices.size();

    vertices.push_back(basePosition + component->position);
    vertices.push_back(basePosition + component->position + glm::vec2(component->size.x, 0.0f));
    vertices.push_back(basePosition + component->position + glm::vec2(0.0f, component->size.y));
    vertices.push_back(basePosition + component->position + component->size);

    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 3);
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 3);
    indices.push_back(baseIndex + 2);

    for (auto i = component->components.begin(); i != component->components.end(); i++)
    {
        UIComponent* childComponent = *i;
        buildVertices(childComponent, basePosition + component->position, vertices, indices);
    }
}