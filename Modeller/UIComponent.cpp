#include "UIComponent.h"

bool UIComponent::checkAndProcessMouseEvent(MouseEvent* mouseEvent)
{
    if (isClicked(mouseEvent))
    {
        return processMouseEvent(mouseEvent);
    }
    return false;
}

bool UIComponent::isClicked(MouseEvent* mouseEvent)
{
    return mouseEvent->position.x > position.x
        && mouseEvent->position.y > position.y
        && mouseEvent->position.x < position.x + size.x
        && mouseEvent->position.y < position.y + size.y;
}