#include "UIRenderOptions.h"

#include <Texture.h>

#include "UIToggleButton.h"

void UIRenderOptions::build()
{
    setPosition(0.01f, 0.01f);

    buttonSize = 0.1f;

    GLuint blankTexture = Texture::getTexture("Blank");

    addToggleButton(renderSurface, glm::vec2(0.01f, 0.01f), blankTexture);
    addToggleButton(renderLines, glm::vec2(0.12f, 0.01f), blankTexture);
    addToggleButton(renderPoints, glm::vec2(0.23f, 0.01f), blankTexture);
    addToggleButton(renderBones, glm::vec2(0.34f, 0.01f), blankTexture);

    setSize(0.45f, 0.12f);
    constructSurface();
    surface.diffuseMap = blankTexture;
}

void UIRenderOptions::addToggleButton(bool& toggle, glm::vec2 position, GLuint texture)
{
    UIToggleButton* button = new UIToggleButton(toggle);
    button->setPositionAndSize(position, glm::vec2(buttonSize, buttonSize));
    button->surface.diffuseMap = texture;
    addComponent(button);
}