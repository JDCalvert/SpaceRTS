#include "UITextBox.h"

UITextBox::UITextBox()
{
    label = new UILabel();
    addComponent(label);

    label->setPosition(glm::vec2());
}

void UITextBox::setText(std::string text, float size, Font font, Alignment alignment)
{
    label->setText(text, size, font, alignment);
}

void UITextBox::setSize(glm::vec2 size)
{
    UIComponent::setSize(size);
    label->setSize(size);
    constructSurface();
}