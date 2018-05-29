#include "UIImportPanel.h"

#include <Texture.h>
#include <UINumber.h>

#include "UserInterfaceModeller.h"

void UIImportPanel::actionPerformed(UIComponent* component)
{
    if (component == importButton)
    {
        std::string fileName = fileTextBox->getText();
        UserInterfaceModeller::getInstance()->importSurface(&fileName[0]);
    }
}

void UIImportPanel::build()
{
    font = &Font::getFont("Calibri");
    textSize = 0.025f;
    width = 0.25f;
    border = 0.01f;

    textureCoordinatesTopLeft = glm::vec2(0.0f);
    textureCoordinatesBottomRight = glm::vec2(1.0f);

    blankTexture = Texture::getTexture("Blank");

    UILabel* fileLabel = new UILabel();
    fileLabel->setSize(0.2f, textSize);
    fileLabel->setText("File", textSize, *font, LEFT);
    addComponent(fileLabel);

    fileTextBox = new UITextBox();
    fileTextBox->setSize(width, textSize);
    fileTextBox->setText("", textSize, *font, LEFT);
    fileTextBox->surface.diffuseMap = blankTexture;
    addComponent(fileTextBox);

    UILabel* texCoordinateLabel = new UILabel();
    texCoordinateLabel->setSize(0.2f, textSize);
    texCoordinateLabel->setText("Texture Coordinates", textSize, *font, LEFT);
    addComponent(texCoordinateLabel);

    UIPanel* textureCoordinatesPanel = new UIPanel();


    addNumber(textureCoordinatesPanel, textureCoordinatesTopLeft.x);
    addNumber(textureCoordinatesPanel, textureCoordinatesTopLeft.y);
    UILabel::addLabel(textureCoordinatesPanel, 0.02f, textSize, "-", *font, CENTRE);
    addNumber(textureCoordinatesPanel, textureCoordinatesBottomRight.x);
    addNumber(textureCoordinatesPanel, textureCoordinatesBottomRight.y);

    ypos += textSize + 0.002f;
    xpos = border;

    importButton = new UIButton(this);
    importButton->setPosition(xpos, ypos);
    importButton->setSize(width, textSize);
    importButton->setText("Import", textSize, *font, CENTRE);
    importButton->surface.diffuseMap = blankTexture;
    addComponent(importButton);

    ypos += textSize + 0.002f;
    xpos += width + border;

    setPosition(border, 0.4f);
    setSize(xpos + border - 0.002f, ypos + border - 0.002f);
    surface.diffuseMap = blankTexture;
}

void UIImportPanel::addNumber(UIPanel* panel, float& number)
{
    UINumber* uiNumber = new UINumber(number);
    uiNumber->setSize(0.055f, textSize);
    uiNumber->setText(textSize, *font, RIGHT);
    uiNumber->surface.diffuseMap = blankTexture;
    panel->addComponent(uiNumber);
}