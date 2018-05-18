#include "UITriangleInformation.h"

#include <sstream>
#include <algorithm>

#include <Texture.h>
#include <UILabel.h>
#include <UIInteger.h>

#include "UITrianglePanel.h"

UITriangleInformation::UITriangleInformation(Surface* infoSurface)
{
    this->infoSurface = infoSurface;

    border = 0.01f;
    indexWidth = 0.05f;
    textSize = 0.025f;

    startTriangle = 0;
    maxTriangles = 36;

    font = &Font::getFont("Calibri");
    blankTexture = Texture::getTexture("Blank");
}

void UITriangleInformation::build()
{
    trianglePanels.clear();
    clearComponents();

    xpos = border;
    ypos = border;

    addHeader("v1");
    addHeader("v2");
    addHeader("v3");

    ypos += textSize;

    std::vector<unsigned int>& indices = infoSurface->getIndices();

    int numTriangles = indices.size();
    int maxTriangle = std::min(startTriangle + maxTriangles, numTriangles);
    for (unsigned int i = startTriangle; i<maxTriangle; i+=3)
    {
        addTrianglePanel(infoSurface, i);
        ypos += textSize + 0.002f;
    }

    setPosition(border, 1.0f - (ypos + border));
    setSize(xpos, ypos + border - 0.002f);
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
    trianglePanel->setPosition(border, ypos);
    trianglePanel->buildPanel();
    addComponent(trianglePanel);

    trianglePanels.push_back(trianglePanel);
}

EventStatus UITriangleInformation::processMouseScroll(MouseScrollEvent mouseEvent)
{
    int numIndices = infoSurface->getIndices().size();

    if (mouseEvent.yOffset > 0
        && startTriangle > 0)
    {
        startTriangle -= 3;
        build();
    }
    else if (mouseEvent.yOffset < 0
        && startTriangle  < numIndices - maxTriangles)
    {
        startTriangle += 3;
        build();
    }

    return PROCESSED;
}

std::vector<UITrianglePanel*>& UITriangleInformation::getTrianglePanels()
{
    return trianglePanels;
}