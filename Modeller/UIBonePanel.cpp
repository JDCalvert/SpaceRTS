#include "UIBonePanel.h"

#include <sstream>

#include <UserInterface.h>
#include <UILabel.h>
#include <UIInteger.h>
#include <UINumber.h>
#include <Texture.h>

#include "UIBoneInformation.h"
#include "UserInterfaceModeller.h"

UIBonePanel::UIBonePanel(UIBoneInformation* parent, Surface* infoSurface, unsigned int index) :
    bone(infoSurface->bones[index])
{
    this->parent = parent;
    this->index = index;

    xpos = 0.0f;
    ypos = 0.0f;

    onMap = Texture::getTexture("Blank");
    offMap = Texture::getTexture("BlankNothing");
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
    addIndexLabel();
    addParentNumber();
    addRemoveButton();

    for (unsigned int i=0; i<4; i++)
    {
        ypos = 0.0f;
        for (unsigned int j=0; j<4; j++)
        {
            addNumber(bone.relative[i][j]);
            ypos += parent->textSize + 0.002f;
        }
        xpos += parent->columnWidth + parent->border;
    }

    setSize(xpos - parent->border, ypos - 0.002f);
    constructSurface();
}

void UIBonePanel::addIndexLabel()
{
    std::stringstream ss;
    ss << index;
    std::string str = ss.str();

    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(0.0f, 0.0f), glm::vec2(parent->indexWidth, parent->textSize));
    label->setText(str, parent->textSize, *parent->font, RIGHT);
    addComponent(label);

    xpos += parent->indexWidth + parent->border;
}

void UIBonePanel::addParentNumber()
{
    UIInteger* uiInteger = new UIInteger(bone.parent);
    uiInteger->setPositionAndSize(glm::vec2(xpos, 0.0f), glm::vec2(parent->indexWidth, parent->textSize));
    uiInteger->setText(parent->textSize, *parent->font, RIGHT);
    uiInteger->surface.diffuseMap = parent->texture;
    addComponent(uiInteger);

    ypos += parent->textSize + 0.002f;
}

void UIBonePanel::addRemoveButton()
{
    removeButton = new UIButton(this);
    removeButton->setPosition(xpos, ypos);
    removeButton->setSize(parent->indexWidth, parent->indexWidth);
    removeButton->setText("R", parent->indexWidth, *parent->font, CENTRE);
    removeButton->surface.diffuseMap = parent->texture;
    addComponent(removeButton);

    xpos += parent->indexWidth + parent->border;
}

void UIBonePanel::addNumber(float& value)
{
    UINumber* uiNumber = new UINumber(value);
    uiNumber->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(parent->columnWidth, parent->textSize));
    uiNumber->setText(parent->textSize, *parent->font, RIGHT);
    uiNumber->surface.diffuseMap = parent->texture;
    uiNumber->setActionListener(this);
    addComponent(uiNumber);
}

void UIBonePanel::processMouseClick(MouseClickEvent mouseEvent)
{
    parent->activeBone = index;
}