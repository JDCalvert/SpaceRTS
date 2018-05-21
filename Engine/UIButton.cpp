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

void UIButton::setText(std::string text, float textSize, Font font, Alignment alignment)
{
    if (!label)
    {
        label = new UILabel();
        label->setSize(size);
        addComponent(label);
    }
    label->setText(text, textSize, font, alignment);
}

void UIButton::setSize(glm::vec2 size)
{
    UIComponent::setSize(size);
    if (label) label->setSize(size);
}