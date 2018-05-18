#include "UISaveLoadPanel.h"

#include <OpenGLContext.h>
#include <UITextBox.h>
#include <UIButton.h>
#include <UILabel.h>

#include <Texture.h>

#include "UserInterfaceModeller.h"

void UISaveLoadPanel::build()
{
    setPosition(0.01f, 0.14f);

    font = &Font::getFont("Calibri");
    textSize = 0.025f;
    textBoxWidth = 0.2f;
    buttonWidth = 0.06f;
    border = 0.01f;

    blankTexture = Texture::getTexture("Blank");

    float xpos = border;
    float ypos = border;

    loadBox = new UITextBox();
    loadBox->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(textBoxWidth, textSize));
    loadBox->setText("", textSize, *font, LEFT);
    loadBox->surface->diffuseMap = blankTexture;
    addComponent(loadBox);

    xpos += textBoxWidth + border;

    loadButton = new UIButton(this);
    loadButton->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(buttonWidth, textSize));
    loadButton->surface->diffuseMap = blankTexture;
    addComponent(loadButton);

    UILabel* loadLabel = new UILabel();
    loadLabel->setPositionAndSize(glm::vec2(), glm::vec2(buttonWidth, textSize));
    loadLabel->setText("Load", textSize, *font, CENTRE);
    loadButton->addComponent(loadLabel);

    xpos = border;
    ypos += textSize + 0.002f;

    saveBox = new UITextBox();
    saveBox->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(textBoxWidth, textSize));
    saveBox->setText("", textSize, *font, LEFT);
    saveBox->surface->diffuseMap = blankTexture;
    addComponent(saveBox);

    xpos += textBoxWidth + border;

    saveButton = new UIButton(this);
    saveButton->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(buttonWidth, textSize));
    saveButton->surface->diffuseMap = blankTexture;
    addComponent(saveButton);

    UILabel* saveLabel = new UILabel();
    saveLabel->setPositionAndSize(glm::vec2(), glm::vec2(buttonWidth, textSize));
    saveLabel->setText("Save", textSize, *font, CENTRE);
    saveButton->addComponent(saveLabel);

    setSize(border * 3 + textBoxWidth + buttonWidth, border + textSize + 0.002f + textSize + border);
    constructSurface();
    surface->diffuseMap = blankTexture;
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