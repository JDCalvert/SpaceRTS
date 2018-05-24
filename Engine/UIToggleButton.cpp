#include "UIToggleButton.h"
#include "Texture.h"

UIToggleButton::UIToggleButton(bool& toggle) : toggle(toggle), UIComponent()
{
    onMap = Texture::getTexture("Blank");
    offMap = Texture::getTexture("BlankDark");
}

void UIToggleButton::processMouseClick(MouseClickEvent mouseEvent)
{
    toggle = !toggle;
}

void UIToggleButton::preRender()
{
    surface.diffuseMap = toggle ? onMap : offMap;
}

bool UIToggleButton::shouldCheckMouseClickEventForChildren()
{
    return false;
}

void UIToggleButton::setText(std::string text, float textSize, Font font, Alignment alignment)
{
    if (!label)
    {
        label = new UILabel();
        label->setSize(size);
        addComponent(label);
    }
    label->setText(text, textSize, font, alignment);
}

void UIToggleButton::setSize(glm::vec2 size)
{
    UIComponent::setSize(size);
    if (label) label->setSize(size);
}