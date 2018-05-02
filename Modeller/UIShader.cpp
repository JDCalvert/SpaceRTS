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
    renderUiComponent(component, glm::vec2(0.0f, 0.0f));
}

void UIShader::renderUiComponent(UIComponent* component, glm::vec2 basePosition)
{
    glUseProgram(programId);
    glBindVertexArray(vertexArrayId);

    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
    buildVertices(component, basePosition, vertices, indices);

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

    GLenum renderMode = component->getRenderMode();
    glDrawElements(renderMode, length, GL_UNSIGNED_INT, 0);

    //Now draw the 
    for (auto i = component->components.begin(); i != component->components.end(); i++)
    {
        UIComponent* childComponent = *i;
        renderUiComponent(childComponent, basePosition + component->position);
    }
}

void UIShader::buildVertices(UIComponent* component, glm::vec2 basePosition, std::vector<glm::vec2> &vertices, std::vector<unsigned int> &indices)
{
    std::vector<glm::vec2> componentVertices = component->getVertices();
    std::vector<unsigned int> componentIndices = component->getIndices();

    for (auto i = componentVertices.begin(); i != componentVertices.end(); i++)
    {
        glm::vec2 vertex = *i;
        vertices.push_back(basePosition + vertex);
    }

    unsigned int baseIndex = indices.size();

    for (auto i = componentIndices.begin(); i != componentIndices.end(); i++)
    {
        unsigned int index = *i;
        indices.push_back(baseIndex + index);
    }        
}