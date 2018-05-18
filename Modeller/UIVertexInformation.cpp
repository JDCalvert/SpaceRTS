#include "UIVertexInformation.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <OpenGLContext.h>
#include <Texture.h>

#include "UILabel.h"
#include "UINumber.h"
#include "UIToggleButton.h"

UIVertexInformation::UIVertexInformation(Surface* infoSurface) : UIInformation(infoSurface)
{
    showVertices = true;
    showTextureCoordinates = false;
    showNormals = false;

    indexWidth = 0.03f;
    columnWidth = 0.075f;

    numItemsDisplay = 12;
}

void UIVertexInformation::preRender()
{
    if (showVertices != previousShowVertices
        || showTextureCoordinates != previousShowTextureCoordinates
        || showNormals != previousShowNormals)
    {
        build();
    }

    setPosition(OpenGLContext::currentContext()->getAspectRatio() - (size.x + 0.01f), 0.01f);

    previousShowVertices = showVertices;
    previousShowTextureCoordinates = showTextureCoordinates;
    previousShowNormals = showNormals;
}

void UIVertexInformation::build()
{
    vertexPanels.clear();
    clearComponents();

    xpos = border;
    float buttonSize = 0.02f;
    addToggleButton(showVertices, xpos, buttonSize, texture);
    addToggleButton(showTextureCoordinates, xpos, buttonSize, texture);
    addToggleButton(showNormals, xpos, buttonSize, texture);

    xpos = border + indexWidth + border;

    addHeaderAndSubHeaders(showVertices, "Vertices", 'X', 3);
    addHeaderAndSubHeaders(showTextureCoordinates, "Texture Coordinates", 'U', 2);
    addHeaderAndSubHeaders(showNormals, "Normals", 'X', 3);
    
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
        addSubHeader(firstSubHeader + i);
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

void UIVertexInformation::addSubHeader(char subHeader)
{
    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xpos, border + textSize), glm::vec2(columnWidth, textSize));
    label->setText(std::string(1, subHeader), textSize, *font, CENTRE);
    addComponent(label);

    xpos += columnWidth + border;
}

void UIVertexInformation::addToggleButton(bool& toggle, float& xpos, float buttonSize, GLuint texture)
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