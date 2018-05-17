#include "UIButton.h"

UIButton::UIButton(UIActionListener* listener)
{
    this->listener = listener;
}

void UIButton::processMouseClick(MouseClickEvent mouseEvent)
{
    listener->actionPerformed(this);
}

bool UIButton::shouldCheckMouseClickEventForChildren()
{
    return false;
}