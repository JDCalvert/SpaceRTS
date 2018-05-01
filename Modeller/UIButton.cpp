#include "UIButton.h"

bool UIButton::processMouseEvent(MouseEvent* mouseEvent)
{
    toggle = !toggle;
    return true;
}