#include "UIComponent.h"

bool UIComponent::checkAndProcessMouseEvent(MouseEvent* mouseEvent)
{
    if (!isClicked(mouseEvent))
    {
        return false;
    }

    MouseEvent* relativeMouseEvent = mouseEvent->getRelative(position);

    //Go through our sub-components and, check if any were clicked. If so process and then return out.
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;
        if (component->checkAndProcessMouseEvent(relativeMouseEvent))
        {
            return true;
        }
    }

    processMouseEvent(mouseEvent);
    return true;
}

bool UIComponent::isClicked(MouseEvent* mouseEvent)
{
    return mouseEvent->position.x > position.x
        && mouseEvent->position.y > position.y
        && mouseEvent->position.x < position.x + size.x
        && mouseEvent->position.y < position.y + size.y;
}

void UIComponent::addComponent(UIComponent* component)
{
    components.push_back(component);
}

void UIComponent::setPosition(glm::vec2 position)
{
    this->position = position;
}

void UIComponent::setSize(glm::vec2 size)
{
    this->size = size;
}

void UIComponent::setPositionAndSize(glm::vec2 position, glm::vec2 size)
{
    setPosition(position);
    setSize(size);
    constructSurface();
}

void UIComponent::constructSurface()
{
    if (!surface) surface = new Surface();

    surface->getVertices() = 
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(glm::vec2(0.0f, size.y), 0.0f),
        glm::vec3(glm::vec2(size.x, 0.0f), 0.0f),
        glm::vec3(size, 0.0f)
    };

    surface->getTextureCoordinates() =
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    surface->getIndices() = {0, 1, 2, 3};

    surface->calculateSizesAndLength();
}

GLenum UIComponent::getRenderMode()
{
    return GL_TRIANGLE_STRIP;
}

glm::vec2 UIComponent::getPosition()
{
    return position;
}

glm::vec2 UIComponent::getSize()
{
    return size;
}