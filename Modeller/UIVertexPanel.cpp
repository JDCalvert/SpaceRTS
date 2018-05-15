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

    setSize(glm::vec2(xpos - parent->border, parent->textSize));
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
        addNumber(row[i], glm::vec2(xpos, 0.0f));
        xpos += parent->columnWidth + parent->border;
    }
}

void UIVertexPanel::addRowVec3(glm::vec3& row)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        addNumber(row[i], glm::vec2(xpos, 0.0f));
        xpos += parent->columnWidth + parent->border;
    }
}

void UIVertexPanel::addNumber(float& number, glm::vec2 position)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3) << number;
    std::string str = ss.str();

    UINumber* numberBox = new UINumber(number);
    numberBox->setPositionAndSize(position, glm::vec2(parent->columnWidth, parent->textSize));
    numberBox->setText(str, parent->textSize, *parent->font, RIGHT);
    numberBox->surface->diffuseMap = parent->blankTexture;
    addComponent(numberBox);
}