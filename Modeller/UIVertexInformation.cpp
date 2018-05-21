#include "UIVertexInformation.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <OpenGLContext.h>
#include <Texture.h>

#include "UILabel.h"
#include "UINumber.h"
#include "UIToggleButton.h"
#include "UserInterfaceModeller.h"

UIVertexInformation::UIVertexInformation(Surface* infoSurface) : UIInformation(infoSurface)
{
    showVertices = true;
    showTextureCoordinates = false;
    showNormals = false;
    showBones = false;

    indexWidth = 0.03f;
    columnWidth = 0.075f;
    buttonSize = 0.02f;
    newButtonXpos = border * 2 + indexWidth;

    numItemsDisplay = 12;
}

void UIVertexInformation::preRender()
{
    if (showVertices != previousShowVertices
        || showTextureCoordinates != previousShowTextureCoordinates
        || showNormals != previousShowNormals
        || showBones != previousShowBones
        || shouldRebuild)
    {
        build();

        previousShowVertices = showVertices;
        previousShowTextureCoordinates = showTextureCoordinates;
        previousShowNormals = showNormals;
        previousShowBones = showBones;

        shouldRebuild = false;
    }

    setPosition(OpenGLContext::currentContext()->getAspectRatio() - (size.x + 0.01f), 0.01f);
}

void UIVertexInformation::build()
{
    vertexPanels.clear();
    clearComponents();

    xpos = border;
    addToggleButton(showVertices);
    addToggleButton(showTextureCoordinates);
    addToggleButton(showNormals);
    addToggleButton(showBones);

    xpos = border + indexWidth + border;

    addHeaderAndSubHeaders(showVertices, "Vertices", 'X', 3);
    addHeaderAndSubHeaders(showTextureCoordinates, "Texture Coordinates", 'U', 2);
    addHeaderAndSubHeaders(showNormals, "Normals", 'X', 3);

    if (showBones)
    {
        addHeader("Bones", 3);
        addSubHeader('i', indexWidth);
        addSubHeader('v', columnWidth);
        addSubHeader('i', indexWidth);
        addSubHeader('v', columnWidth);
    }

    ypos = border + textSize * 2;
    addPanels();
}

UIComponent* UIVertexInformation::addPanel(unsigned int i)
{
    UIVertexPanel* vertexPanel = new UIVertexPanel(this, infoSurface, i);
    vertexPanel->setPosition(border, ypos);
    vertexPanel->buildPanel();
    addComponent(vertexPanel);

    vertexPanels.push_back(vertexPanel);

    return vertexPanel;
}

void UIVertexInformation::addHeaderAndSubHeaders(bool shouldAdd, std::string header, char firstSubHeader, int numSubHeaders)
{
    if (!shouldAdd) return;
    
    addHeader(header, numSubHeaders);
    for (int i = 0; i<numSubHeaders; i++)
    {
        addSubHeader(firstSubHeader + i, columnWidth);
    }
}

void UIVertexInformation::addHeader(std::string text, int numColumns)
{
    float labelWidth = numColumns * (columnWidth + border) - border;

    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xpos, border), glm::vec2(labelWidth, textSize));
    label->setText(text, textSize, *font, CENTRE);
    addComponent(label);
}

void UIVertexInformation::addSubHeader(char subHeader, float width)
{
    addSubHeader(std::string(1, subHeader), width);
}

void UIVertexInformation::addSubHeader(std::string subHeader, float width)
{
    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xpos, border + textSize), glm::vec2(width, textSize));
    label->setText(subHeader, textSize, *font, CENTRE);
    addComponent(label);

    xpos += width + border;
}

void UIVertexInformation::addToggleButton(bool& toggle)
{
    UIToggleButton* button = new UIToggleButton(toggle);
    button->setPositionAndSize(glm::vec2(xpos, 0.01f), glm::vec2(buttonSize, buttonSize));
    button->surface->diffuseMap = texture;
    addComponent(button);

    xpos += buttonSize + border;
}

std::vector<UIVertexPanel*>& UIVertexInformation::getVertexPanels()
{
    return vertexPanels;
}

int UIVertexInformation::getNumItemsTotal()
{
    return infoSurface->getVertices().size();
}

void UIVertexInformation::actionPerformed(UIComponent* component)
{
    if (component == newButton)
    {
        UserInterfaceModeller::getInstance()->newVertex();

        currentItem = std::max(0, numItemsTotal - numItemsDisplay + 1);
        shouldRebuild = true;
    }
}