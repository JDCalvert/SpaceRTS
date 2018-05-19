#include "UIVertexPanel.h"

#include <sstream>
#include <iomanip>

#include <Texture.h>

#include "UILabel.h"
#include "UINumber.h"
#include "UIVertexInformation.h"
#include "UserInterface.h"

UIVertexPanel::UIVertexPanel(UIVertexInformation* parent, Surface* infoSurface, unsigned int index) :
    vertexPosition(infoSurface->getVertices()[index]),
    vertexTextureCoordinate(infoSurface->getTextureCoordinates()[index]),
    vertexNormal(infoSurface->getNormals()[index]),
    vertexBoneIndices(infoSurface->getBoneIndicesAndWeights()[index]),
    UIPanel()
{
    this->parent = parent;
    this->index = index;

    onMap = Texture::getTexture("Blank");
    offMap = Texture::getTexture("BlankNothing");
    
    xpos = 0.0f;
}

bool UIVertexPanel::isHighlighted()
{
    return highlighted;
}

void UIVertexPanel::preRender()
{
    UIComponent* component = UserInterface::getInstance()->getActiveComponent();
    highlighted = hover || isChild(component);
    surface->diffuseMap = highlighted ? onMap : offMap;
}

void UIVertexPanel::buildPanel()
{
    addIndexLabel();
    if (parent->showVertices) addRowVec3(vertexPosition);
    if (parent->showTextureCoordinates) addRowVec2(vertexTextureCoordinate);
    if (parent->showNormals) addRowVec3(vertexNormal);
    if (parent->showBones) addBones();

    setSize(xpos - parent->border, parent->textSize);
    constructSurface();
}

void UIVertexPanel::addIndexLabel()
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

void UIVertexPanel::addRowVec2(glm::vec2& row)
{
    for (unsigned int i = 0; i < 2; i++)
    {
        addNumber(row[i], 3, parent->columnWidth);
    }
}

void UIVertexPanel::addRowVec3(glm::vec3& row)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        addNumber(row[i], 3, parent->columnWidth);
    }
}

void UIVertexPanel::addBones()
{
    addNumber(vertexBoneIndices[0], 0, parent->indexWidth);
    addNumber(vertexBoneIndices[1], 3, parent->columnWidth);
    addNumber(vertexBoneIndices[2], 0, parent->indexWidth);
    addNumber(vertexBoneIndices[3], 3, parent->columnWidth);
}

void UIVertexPanel::addNumber(float& number, int numDigits, float width)
{
    UINumber* numberBox = new UINumber(number);
    numberBox->setPositionAndSize(glm::vec2(xpos, 0.0f), glm::vec2(width, parent->textSize));
    numberBox->setNumDigits(numDigits);
    numberBox->setText(parent->textSize, *parent->font, RIGHT);
    numberBox->surface->diffuseMap = parent->texture;
    addComponent(numberBox);

    xpos += width + parent->border;
}