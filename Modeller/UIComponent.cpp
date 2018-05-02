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

std::vector<glm::vec2> UIComponent::getVertices()
{
    return std::vector<glm::vec2>
    {
        position,
        position + glm::vec2(size.x, 0.0f),
        position + glm::vec2(0.0f, size.y),
        position + size
        
    };
}

std::vector<unsigned int> UIComponent::getIndices()
{
    return std::vector<unsigned int>
    {
        0, 1, 2, 3
    };
}

GLenum UIComponent::getRenderMode()
{
    return GL_TRIANGLE_STRIP;
}