#include "UITriangleInformation.h"

#include <sstream>
#include <algorithm>

#include <UILabel.h>

#include "UITrianglePanel.h"

UITriangleInformation::UITriangleInformation(Surface* infoSurface) : UIInformation(infoSurface)
{
    indexWidth = 0.05f;
    numItemsDisplay = 12;
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

    addPanels();

    setPosition(border, 1.0f - (size.y + border));
}

void UITriangleInformation::preRender()
{
    if (shouldRebuild)
    {
        build();
        shouldRebuild = false;
    }
}

void UITriangleInformation::addHeader(std::string header)
{
    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(indexWidth, textSize));
    label->setText(header, textSize, *font, CENTRE);
    addComponent(label);

    xpos += indexWidth + border;
}

UIComponent* UITriangleInformation::addPanel(unsigned int index)
{
    UITrianglePanel* trianglePanel = new UITrianglePanel(this, infoSurface, index);
    trianglePanel->setPosition(border, ypos);
    trianglePanel->buildPanel();
    addComponent(trianglePanel);

    trianglePanels.push_back(trianglePanel);

    return trianglePanel;
}

std::vector<UITrianglePanel*>& UITriangleInformation::getTrianglePanels()
{
    return trianglePanels;
}

int UITriangleInformation::getNumItemsTotal()
{
    return infoSurface->getIndices().size() / 3;
}

void UITriangleInformation::actionPerformed(UIComponent* component)
{
    if (component == newButton)
    {
        std::vector<unsigned int>& indices = infoSurface->getIndices();
        indices.push_back(0);
        indices.push_back(0);
        indices.push_back(0);

        infoSurface->calculateSizesAndLength();
        currentItem = std::max(0, numItemsTotal - numItemsDisplay + 1);
        shouldRebuild = true;
    }
}