#include "UIButton.h"

UIButton::UIButton(UIActionListener* listener)
{
    this->listener = listener;
}

void UIButton::processMouseEvent(MouseClickEvent* mouseEvent)
{
    listener->actionPerformed(this);
}