#include "UITextBox.h"

#include <glfw\glfw3.h>

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

bool UITextBox::shouldCheckMouseEventForChildren()
{
    return false;
}

bool UITextBox::shouldRemainActive()
{
    return true;
}

void UITextBox::processKeyEvent(KeyEvent* event)
{
    if (event->action != GLFW_PRESS && event->action != GLFW_REPEAT) return;

    if (event->key == GLFW_KEY_BACKSPACE)
    {
        label->removeLastChar();
    }
    else if (event->key == GLFW_KEY_ENTER
      || event->key == GLFW_KEY_KP_ENTER)
    {
        becomeInactive();
    }
}

void UITextBox::processTextEvent(TextEvent* event)
{
    label->addChar(event->codepoint);
}