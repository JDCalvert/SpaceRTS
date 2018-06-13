#include "UIBonePanel.h"

#include <sstream>

#include <UserInterface.h>
#include <UILabel.h>
#include <UIInteger.h>
#include <UINumber.h>
#include <Texture.h>
#include <UILayoutFormVertical.h>
#include <UILayoutHorizontal.h>

#include "UIBoneInformation.h"
#include "UserInterfaceModeller.h"

UIBonePanel::UIBonePanel(UIBoneInformation* parent, Surface* infoSurface, unsigned int index) :
    bone(infoSurface->bones[index])
{
    this->parent = parent;
    this->index = index;

    onMap = Texture::getTexture("Blank");
    offMap = Texture::getTexture("BlankNothing");
}

unsigned int UIBonePanel::getIndex()
{
    return index;
}

bool UIBonePanel::isHighlighted()
{
    return highlighted;
}

void UIBonePanel::preRender()
{
    UIComponent* component = UserInterface::getInstance()->getActiveComponent();
    highlighted = hover || isChild(component) || parent->activeBone == index;
    surface.diffuseMap = highlighted ? onMap : offMap;
}

void UIBonePanel::actionPerformed(UIComponent* component)
{
    if (component == removeButton)
    {
        UserInterfaceModeller::getInstance()->removeBone(index);
    }
    else
    {
        UserInterfaceModeller::getInstance()->recalculateVertexPositions();
    }
}

void UIBonePanel::buildPanel()
{
    layout = new UILayoutFormVertical(this, 4);
    layout->externalBorder = glm::vec2(0.0f);

    addIndexPanel();
    addRemoveButton();
    addComponent(new UIPanel());
    addComponent(new UIPanel());

    for (unsigned int i=0; i<4; i++)
    {
        for (unsigned int j=0; j<4; j++)
        {
            addNumber(bone.relative[i][j]);
        }
    }

    if (parent->showTextureCoordinates)
    {
        UILabel* textureCoordinatesLabel = new UILabel();
        textureCoordinatesLabel->setText("Texture Coordinates", parent->textSize, *parent->font, CENTRE);
        textureCoordinatesLabel->setSizeFromText();
        addComponent(textureCoordinatesLabel);

        addTextureCoordinatePanel(textureCoordinateTopLeft);

        UILabel* textureCoordinatesToLabel = new UILabel();
        textureCoordinatesToLabel->setText("to", parent->textSize, *parent->font, CENTRE);
        textureCoordinatesToLabel->setSizeFromText();
        addComponent(textureCoordinatesToLabel);

        addTextureCoordinatePanel(textureCoordinateBottomRight);
    }

    recalculateLayout();
}

void UIBonePanel::addIndexPanel()
{
    UIPanel* topPanel = new UIPanel();
    topPanel->layout = new UILayoutHorizontal(topPanel);
    topPanel->layout->externalBorder = glm::vec2(0.0f, 0.0f);
    addComponent(topPanel);

    std::stringstream ss;
    ss << index;
    std::string str = ss.str();

    UILabel* label = new UILabel();
    label->setSize(parent->indexWidth, parent->textSize);
    label->setText(str, parent->textSize, *parent->font, RIGHT);
    topPanel->addComponent(label);

    UIInteger* uiInteger = new UIInteger(bone.parent);
    uiInteger->setSize(parent->indexWidth, parent->textSize);
    uiInteger->setText(parent->textSize, *parent->font, RIGHT);
    uiInteger->surface.diffuseMap = parent->texture;
    topPanel->addComponent(uiInteger);
}

void UIBonePanel::addRemoveButton()
{
    //Don't add the remove button for the base index, we always need it
    if (index == 0)
    {
        addComponent(new UIPanel());
    }
    else
    {
        removeButton = new UIButton(this);
        removeButton->setText("Remove", parent->textSize, *parent->font, CENTRE);
        removeButton->setSizeFromText();
        removeButton->surface.diffuseMap = parent->texture;
        addComponent(removeButton);
    }
}

void UIBonePanel::addNumber(float& value)
{
    UINumber* uiNumber = new UINumber(value);
    uiNumber->setSize(parent->columnWidth, parent->textSize);
    uiNumber->setText(parent->textSize, *parent->font, RIGHT);
    uiNumber->surface.diffuseMap = parent->texture;
    uiNumber->setActionListener(this);
    addComponent(uiNumber);
}

void UIBonePanel::addTextureCoordinatePanel(glm::vec2& textureCoordinate)
{
    UIPanel* textureCoordinatePanel = new UIPanel();
    textureCoordinatePanel->layout = new UILayoutHorizontal(textureCoordinatePanel);
    addComponent(textureCoordinatePanel);

    UINumber* textureCoordinateX = new UINumber(textureCoordinate[0]);
    textureCoordinateX->setText(parent->textSize, *parent->font, RIGHT);
    textureCoordinateX->setSize(0.035f, parent->textSize);
    textureCoordinateX->surface.diffuseMap = parent->texture;
    textureCoordinatePanel->addComponent(textureCoordinateX);

    UILabel* textureCoordinateDash = new UILabel();
    textureCoordinateDash->setText("-", parent->textSize, *parent->font, CENTRE);
    textureCoordinateDash->setSizeFromText();
    textureCoordinatePanel->addComponent(textureCoordinateDash);

    UINumber* textureCoordinateY = new UINumber(textureCoordinate[1]);
    textureCoordinateY->setText(parent->textSize, *parent->font, RIGHT);
    textureCoordinateY->setSize(0.035f, parent->textSize);
    textureCoordinateY->surface.diffuseMap = parent->texture;
    textureCoordinatePanel->addComponent(textureCoordinateY);
}

void UIBonePanel::processMouseClick(MouseClickEvent mouseEvent)
{
    parent->activeBone = index;
}