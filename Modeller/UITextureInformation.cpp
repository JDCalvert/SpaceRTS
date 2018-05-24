#include "UITextureInformation.h"

#include <algorithm>

#include <Texture.h>

#include "UserInterfaceModeller.h"

UITextureInformation::UITextureInformation(Surface* infoSurface) : UIPanel()
{
    this->infoSurface = infoSurface;

    showSpecular = false;
    showNormal = false;

    buttonSize = 0.02f;
    textureSize = 0.2f;
    height = 0.0f;

    surface.diffuseMap = Texture::getTexture("Blank");    
}

void UITextureInformation::build()
{
    xpos = textureSize - 0.032f;

    addToggleButton(showSpecularButton, showSpecular, "S");
    addToggleButton(showNormalButton, showNormal, "N");

    addTexturePanel(diffusePanel, infoSurface->diffuseMap, "Diffuse");
    addTexturePanel(specularPanel, infoSurface->specularMap, "Specular");
    addTexturePanel(normalPanel, infoSurface->normalMap, "Normal");

    setSize(xpos, height);

    rebuildPanel();
}

void UITextureInformation::rebuildPanel()
{
    components.clear();
    addComponent(showSpecularButton);
    addComponent(showNormalButton);

    xpos = 0.01f;

    replaceTexturePanel(diffusePanel);
    if (showSpecular) replaceTexturePanel(specularPanel);
    if (showNormal) replaceTexturePanel(normalPanel);

    setSize(xpos, height);
}

void UITextureInformation::preRender()
{
    if (showSpecular != previousShowSpecular
     || showNormal != previousShowNormal)
    {
        rebuildPanel();

        previousShowSpecular = showSpecular;
        previousShowNormal = showNormal;
    }

    float xpos = UserInterfaceModeller::getInstance()->triangleInformation->getSize().x + 0.02f;;
    float ypos = 0.99f - height;
    setPosition(xpos, ypos);
}

void UITextureInformation::addTexturePanel(UITexturePanel*& texturePanel, GLuint& textureId, std::string textureName)
{
    texturePanel = new UITexturePanel(this, textureId, textureName);
    texturePanel->build();
    height = std::max(height, texturePanel->getSize().y + 0.02f);
}

void UITextureInformation::addToggleButton(UIToggleButton*& toggleButton, bool& toggle, std::string buttonText)
{
    toggleButton = new UIToggleButton(toggle);
    toggleButton->setPosition(xpos, 0.01f);
    toggleButton->setSize(buttonSize, buttonSize);
    toggleButton->setText(buttonText, buttonSize, Font::getFont("Calibri"), CENTRE);

    xpos += buttonSize + 0.002f;
}

void UITextureInformation::replaceTexturePanel(UITexturePanel* texturePanel)
{
    texturePanel->setPosition(xpos, 0.01f);
    addComponent(texturePanel);
    xpos += textureSize + 0.01f;
}