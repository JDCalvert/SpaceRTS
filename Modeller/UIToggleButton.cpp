#include "UIToggleButton.h"

UIToggleButton::UIToggleButton(bool& toggle) : toggle(toggle), UIComponent()
{
}

void UIToggleButton::processMouseEvent(MouseEvent* mouseEvent)
{
    toggle = !toggle;
}