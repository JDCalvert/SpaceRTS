#include "UIImportPanel.h"

#include <Texture.h>
#include <UINumber.h>
#include <UILayoutHorizontal.h>
#include <UILayoutVertical.h>

#include "UserInterfaceModeller.h"

void UIImportPanel::actionPerformed(UIComponent* component)
{
    if (component == importButton)
    {
        std::string fileName = fileTextBox->getText();
        UserInterfaceModeller::getInstance()->importSurface(&fileName[0]);
    }
    else if (std::find(numberPanels.begin(), numberPanels.end(), component) != numberPanels.end())
    {
        UserInterfaceModeller::getInstance()->textureInformation->rebuildImportTextureCoordinates();
    }
}

void UIImportPanel::build()
{
    font = &Font::getFont("Calibri");
    textSize = 0.025f;

    textureCoordinatesTopLeft = glm::vec2(0.0f);
    textureCoordinatesBottomRight = glm::vec2(1.0f);

    blankTexture = Texture::getTexture("Blank");

    layout = new UILayoutVertical(this);

    UILabel::addLabel(this, 0.2f, textSize, "File", *font, LEFT);

    fileTextBox = new UITextBox();
    fileTextBox->setSize(0.0f, textSize);
    fileTextBox->setText("", textSize, *font, LEFT);
    fileTextBox->surface.diffuseMap = blankTexture;
    addComponent(fileTextBox);

    UILabel::addLabel(this, 0.2f, textSize, "Texture Coordinates", *font, LEFT);

    UIPanel* textureCoordinatesPanel = new UIPanel();
    textureCoordinatesPanel->surface.diffuseMap = Texture::getTexture("BlankNothing");
    addComponent(textureCoordinatesPanel);

    UILayoutHorizontal* textureCoordinatesLayout = new UILayoutHorizontal(textureCoordinatesPanel);
    textureCoordinatesLayout->externalBorder = glm::vec2(0.0f);
    textureCoordinatesPanel->layout = textureCoordinatesLayout;

    addNumber(textureCoordinatesPanel, textureCoordinatesTopLeft.x);
    addNumber(textureCoordinatesPanel, textureCoordinatesTopLeft.y);
    UILabel::addLabel(textureCoordinatesPanel, 0.02f, textSize, "-", *font, CENTRE);
    addNumber(textureCoordinatesPanel, textureCoordinatesBottomRight.x);
    addNumber(textureCoordinatesPanel, textureCoordinatesBottomRight.y);

    importButton = new UIButton(this);
    importButton->setSize(0.0f, textSize);
    importButton->setText("Import", textSize, *font, CENTRE);
    importButton->surface.diffuseMap = blankTexture;
    addComponent(importButton);

    UserInterfaceModeller* ui = UserInterfaceModeller::getInstance();
    float ypos = ui->renderOptions->getSize().y + ui->saveLoadPanel->getSize().y + 0.03f;;

    setPosition(0.01f, ypos);
    surface.diffuseMap = blankTexture;

    recalculateLayout();
}

void UIImportPanel::addNumber(UIPanel* panel, float& number)
{
    UINumber* uiNumber = new UINumber(number);
    uiNumber->setSize(0.055f, textSize);
    uiNumber->setText(textSize, *font, RIGHT);
    uiNumber->surface.diffuseMap = blankTexture;
    panel->addComponent(uiNumber);

    uiNumber->setActionListener(this);
    numberPanels.push_back(uiNumber);
}