#include "UIToggleButton.h"
#include "Texture.h"

UIToggleButton::UIToggleButton(bool& toggle) : toggle(toggle), UIComponent()
{
    onMap = Texture::getTexture("Blank");
    offMap = Texture::getTexture("BlankDark");
}

void UIToggleButton::processMouseClick(MouseClickEvent* mouseEvent)
{
    toggle = !toggle;
}

void UIToggleButton::preRender()
{
    surface->diffuseMap = toggle ? onMap : offMap;
}