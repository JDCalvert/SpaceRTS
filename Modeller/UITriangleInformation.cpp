#include "UITriangleInformation.h"

#include <sstream>

#include <Texture.h>
#include <UILabel.h>
#include <UIInteger.h>

#include "UITrianglePanel.h"

void UITriangleInformation::build(Surface* infoSurface)
{
    this->infoSurface = infoSurface;

    border = 0.01f;
    indexWidth = 0.05f;
    textSize = 0.025f;

    font = &Font::getFont("Calibri");
    blankTexture = Texture::getTexture("Blank");

    rebuildPanels();
}

void UITriangleInformation::rebuildPanels()
{
    clearComponents();

    xpos = border;
    ypos = border;

    addHeader("v1");
    addHeader("v2");
    addHeader("v3");

    ypos += textSize;

    std::vector<unsigned int>& indices = infoSurface->getIndices();
    for (unsigned int i=0; i<indices.size(); i+=3)
    {
        addTrianglePanel(infoSurface, i);
        ypos += textSize + 0.002f;
    }

    setPosition(glm::vec2(border, 1.0f - (ypos + border)));
    setSize(glm::vec2(xpos, ypos + border - 0.002f));
    constructSurface();
    surface->diffuseMap = blankTexture;
}

void UITriangleInformation::addHeader(std::string header)
{
    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(indexWidth, textSize));
    label->setText(header, textSize, *font, CENTRE);
    addComponent(label);

    xpos += indexWidth + border;
}

void UITriangleInformation::addTrianglePanel(Surface* infoSurface, unsigned int firstIndex)
{
    UITrianglePanel* trianglePanel = new UITrianglePanel(this, infoSurface, firstIndex);
    trianglePanel->setPosition(glm::vec2(border, ypos));
    trianglePanel->buildPanel();
    addComponent(trianglePanel);

    trianglePanels.push_back(trianglePanel);
}

std::vector<UITrianglePanel*>& UITriangleInformation::getTrianglePanels()
{
    return trianglePanels;
}