#include "UISaveLoadPanel.h"

#include <OpenGLContext.h>
#include <UITextBox.h>
#include <UIButton.h>
#include <UILabel.h>

#include <Texture.h>

#include "UserInterfaceModeller.h"

void UISaveLoadPanel::build()
{
    font = &Font::getFont("Calibri");
    textSize = 0.025f;
    textBoxWidth = 0.2f;
    buttonWidth = 0.06f;
    border = 0.01f;

    blankTexture = Texture::getTexture("Blank");

    setPosition(0.01f, 0.14f);

    float ypos = border;

    addBoxAndButton(loadBox, loadButton, "Load", ypos);
    addBoxAndButton(saveBox, saveButton, "Save", ypos);

    setSize(border * 3 + textBoxWidth + buttonWidth, ypos + border - 0.002f);
    surface.diffuseMap = blankTexture;
}

void UISaveLoadPanel::addBoxAndButton(UITextBox*& textBox, UIButton*& button, std::string buttonText, float& ypos)
{
    float xpos = border;

    textBox = new UITextBox();
    textBox->setPosition(xpos, ypos);
    textBox->setSize(textBoxWidth, textSize);
    textBox->setText("", textSize, *font, LEFT);
    textBox->surface.diffuseMap = blankTexture;
    addComponent(textBox);

    xpos += textBoxWidth + border;

    button = new UIButton(this);
    button->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(buttonWidth, textSize));
    button->setText(buttonText, textSize, *font, CENTRE);
    button->surface.diffuseMap = blankTexture;
    addComponent(button);

    ypos += textSize + 0.002f;
}

void UISaveLoadPanel::actionPerformed(UIComponent* component)
{
    if (component == loadButton)
    {
        std::string fileName = loadBox->getText();
        UserInterfaceModeller::getInstance()->loadSurface(&fileName[0]);
    }
    else if (component == saveButton)
    {
        std::string fileName = saveBox->getText();
        UserInterfaceModeller::getInstance()->saveSurface(&fileName[0]);
    }
}