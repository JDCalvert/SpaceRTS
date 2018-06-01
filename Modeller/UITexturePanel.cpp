#include "UITexturePanel.h"

#include <Texture.h>

#include "UserInterfaceModeller.h"
#include "UITextureInformation.h"

UITexturePanel::UITexturePanel(UITextureInformation* parent, GLuint& textureId, std::string& textureName, GLuint textureCoordinateId, std::string labelText) :
    UIPanel(),
    textureId(textureId),
    textureName(textureName)
{
    this->parent = parent;
    this->labelText = labelText;
    this->textureCoordinateId = textureCoordinateId;

    loadButtonWidth = 0.05f;

    textSize = 0.025f;
    border = 0.01f;
    font = &Font::getFont("Calibri");
    texture = Texture::getTexture("Blank");

    surface.diffuseMap = Texture::getTexture("BlankNothing");

    shouldRebuild = false;
}

void UITexturePanel::preRender()
{
    texturePanel->surface.diffuseMap = textureId;
}

void UITexturePanel::build()
{
    clearComponents();

    float xpos = 0.0f;
    float ypos = 0.0f;

    float textBoxWidth = parent->textureSize - loadButtonWidth - 0.002f;

    UILabel* label = new UILabel();
    label->setPosition(xpos, ypos);
    label->setSize(parent->textureSize, textSize);
    label->setText(labelText, textSize, *font, LEFT);
    addComponent(label);

    ypos += textSize;

    textureNameBox = new UITextBox();
    textureNameBox->setPosition(xpos, ypos);
    textureNameBox->setSize(textBoxWidth, textSize);
    textureNameBox->setText(getShortTextureName(), textSize, *font, LEFT);
    textureNameBox->surface.diffuseMap = texture;
    addComponent(textureNameBox);

    xpos += textBoxWidth + 0.002f;

    textureLoadButton = new UIButton(this);
    textureLoadButton->setPosition(xpos, ypos);
    textureLoadButton->setSize(loadButtonWidth, textSize);
    textureLoadButton->setText("Load", textSize, *font, CENTRE);
    textureLoadButton->surface.diffuseMap = texture;
    addComponent(textureLoadButton);

    xpos = 0.0f;
    ypos += textSize + 0.002f;

    texturePanel = new UIPanel();
    texturePanel->setPosition(xpos, ypos);
    texturePanel->setSize(parent->textureSize, parent->textureSize);
    texturePanel->surface.diffuseMap = textureId;
    addComponent(texturePanel);

    textureCoordinatePanel = new UIPanel();
    textureCoordinatePanel->setPosition(xpos, ypos);
    textureCoordinatePanel->setSize(parent->textureSize, parent->textureSize);
    textureCoordinatePanel->surface.diffuseMap = textureCoordinateId;
    addComponent(textureCoordinatePanel);

    xpos += parent->textureSize;
    ypos += parent->textureSize;

    setSize(xpos, ypos);
}

void UITexturePanel::actionPerformed(UIComponent* component)
{
    if (component == textureLoadButton)
    {
        std::string textureStr = texturePrefix + textureNameBox->getText() + textureSuffix;
        GLuint newTexture = Texture::getTexture(textureStr);
        if (newTexture != -1)
        {
            textureName = textureStr;

            textureId = newTexture;
            shouldRebuild = true;
        }
    }
}

std::string UITexturePanel::getShortTextureName()
{
    if (textureName.length() == 0)
    {
        return "";
    }

    unsigned int start = texturePrefix.length();
    unsigned int end = textureName.find(textureSuffix);
    return textureName.substr(start, end - start);
}

void UITexturePanel::updateTexture()
{
    textureNameBox->setText(getShortTextureName());
}