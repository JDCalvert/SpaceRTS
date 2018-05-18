#include "UITrianglePanel.h"

#include <sstream>

#include <UserInterface.h>
#include <UIInteger.h>
#include <Texture.h>

#include "UITriangleInformation.h"

UITrianglePanel::UITrianglePanel(UITriangleInformation* parent, Surface* infoSurface, unsigned int firstIndex) :
    index1(infoSurface->getIndices()[firstIndex]),
    index2(infoSurface->getIndices()[firstIndex + 1]),
    index3(infoSurface->getIndices()[firstIndex + 2])
{
    this->parent = parent;

    onMap = Texture::getTexture("Blank");
    offMap = Texture::getTexture("BlankNothing");

    xpos = 0.0f;
}

void UITrianglePanel::buildPanel()
{
    addNumber(index1);
    addNumber(index2);
    addNumber(index3);

    setSize(xpos - parent->border, parent->textSize);
    constructSurface();
}

bool UITrianglePanel::isHighlighted()
{
    return highlighted;
}

void UITrianglePanel::preRender()
{
    UIComponent* component = UserInterface::getInstance()->getActiveComponent();
    highlighted = hover || isChild(component);
    surface->diffuseMap = highlighted ? onMap : offMap;
}

void UITrianglePanel::addNumber(unsigned int& index)
{
    std::stringstream ss;
    ss << index;
    std::string str = ss.str();

    UIInteger* uiInteger = new UIInteger(index);
    uiInteger->setPositionAndSize(glm::vec2(xpos, 0.0f), glm::vec2(parent->indexWidth, parent->textSize));
    uiInteger->setText(str, parent->textSize, *parent->font, RIGHT);
    uiInteger->surface->diffuseMap = parent->blankTexture;
    addComponent(uiInteger);

    xpos += parent->indexWidth + parent->border;
}

void UITrianglePanel::addIndices(std::vector<unsigned int>& indices)
{
    indices.push_back(index1);
    indices.push_back(index2);
    indices.push_back(index3);
}