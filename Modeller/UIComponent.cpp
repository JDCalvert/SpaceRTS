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