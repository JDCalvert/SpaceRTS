#include "UITexturePanel.h"

#include <Texture.h>

#include "UserInterfaceModeller.h"
#include "UITextureInformation.h"

UITexturePanel::UITexturePanel(UITextureInformation* parent, GLuint& textureId, std::string labelText) :
    UIPanel(),
    textureId(textureId)
{
    this->parent = parent;
    this->labelText = labelText;

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

    diffuseTextureBox = new UITextBox();
    diffuseTextureBox->setPosition(xpos, ypos);
    diffuseTextureBox->setSize(textBoxWidth, textSize);
    diffuseTextureBox->setText("", textSize, *font, LEFT);
    diffuseTextureBox->surface.diffuseMap = texture;
    addComponent(diffuseTextureBox);

    xpos += textBoxWidth + 0.002f;

    diffuseTextureLoadButton = new UIButton(this);
    diffuseTextureLoadButton->setPosition(xpos, ypos);
    diffuseTextureLoadButton->setSize(loadButtonWidth, textSize);
    diffuseTextureLoadButton->setText("Load", textSize, *font, CENTRE);
    diffuseTextureLoadButton->surface.diffuseMap = texture;
    addComponent(diffuseTextureLoadButton);

    xpos = 0.0f;
    ypos += textSize + 0.002f;

    texturePanel = new UIPanel();
    texturePanel->setPosition(xpos, ypos);
    texturePanel->setSize(parent->textureSize, parent->textureSize);
    texturePanel->surface.diffuseMap = textureId;
    addComponent(texturePanel);

    xpos += parent->textureSize;
    ypos += parent->textureSize;

    setSize(xpos, ypos);
}

void UITexturePanel::actionPerformed(UIComponent* component)
{
    if (component == diffuseTextureLoadButton)
    {
        std::string textureStr = diffuseTextureBox->getText();
        GLuint newTexture = Texture::getTexture(textureStr);
        if (newTexture != -1)
        {
            textureId = newTexture;
            shouldRebuild = true;
        }
    }
}