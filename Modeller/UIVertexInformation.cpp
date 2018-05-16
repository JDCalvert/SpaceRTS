#include "UIVertexInformation.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <OpenGLContext.h>
#include <Texture.h>

#include "UILabel.h"
#include "UINumber.h"
#include "UIToggleButton.h"

UIVertexInformation::UIVertexInformation(Surface* infoSurface)
{
    this->infoSurface = infoSurface;

    showVertices = true;
    showTextureCoordinates = false;
    showNormals = false;

    border = 0.01f;
    textSize = 0.025f;
    indexWidth = 0.03f;
    columnWidth = 0.075f;

    startVertex = 0;
    maxVertices = 12;

    font = &Font::getFont("Calibri");
    blankTexture = Texture::getTexture("Blank");
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
    addToggleButton(showVertices, xpos, buttonSize, blankTexture);
    addToggleButton(showTextureCoordinates, xpos, buttonSize, blankTexture);
    addToggleButton(showNormals, xpos, buttonSize, blankTexture);

    xpos = border + indexWidth + border;

    addHeaderAndSubHeaders(showVertices, "Vertices", 'X', 3);
    addHeaderAndSubHeaders(showTextureCoordinates, "Texture Coordinates", 'U', 2);
    addHeaderAndSubHeaders(showNormals, "Normals", 'X', 3);
    
    float ypos = border + textSize * 2;

    std::vector<glm::vec3>& vertices = infoSurface->getVertices();
    int numVertices = vertices.size();
    int maxVertex = std::min(startVertex + maxVertices, numVertices);
    for (unsigned int i=startVertex; i<maxVertex; i++)
    {
        addVertexPanel(i, ypos);
        ypos += textSize + 0.002f;
    }

    setSize(xpos, ypos + border - 0.002f);
    constructSurface();
    surface->diffuseMap = blankTexture;
}

void UIVertexInformation::addVertexPanel(unsigned int i, float ypos)
{
    UIVertexPanel* vertexPanel = new UIVertexPanel(this, infoSurface, i);
    vertexPanel->setPosition(border, ypos);
    vertexPanel->buildPanel();
    addComponent(vertexPanel);

    vertexPanels.push_back(vertexPanel);
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

bool UIVertexInformation::processMouseScroll(MouseScrollEvent* mouseEvent)
{
    int numVertices = infoSurface->getVertices().size();

    if (mouseEvent->yOffset > 0
     && startVertex > 0)
    {
        startVertex--;
        build();
    }
    else if (mouseEvent->yOffset < 0
        && startVertex  < numVertices - maxVertices)
    {
        startVertex++;
        build();
    }

    return true;
}

std::vector<UIVertexPanel*>& UIVertexInformation::getVertexPanels()
{
    return vertexPanels;
}