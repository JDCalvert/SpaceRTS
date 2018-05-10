#include "UIVertexInformation.h"

#include <sstream>
#include <iomanip>

#include <OpenGLContext.h>
#include <Font.h>
#include <Texture.h>

#include "UILabel.h"
#include "UINumber.h"
#include "UIToggleButton.h"

void UIVertexInformation::build(Surface* infoSurface)
{
    this->infoSurface = infoSurface;

    showVertices = true;
    showTextureCoordinates = false;
    showNormals = false;

    border = 0.01f;
    textSize = 0.025f;
    indexWidth = 0.03f;
    columnWidth = 0.075f;

    font = &Font::getFont("Calibri");

    rebuildPanels();
}

void UIVertexInformation::preRender()
{
    setPosition(glm::vec2(OpenGLContext::currentContext()->getAspectRatio() - (size.x + 0.01f), 0.01f));

    if (showVertices != previousShowVertices
     || showTextureCoordinates != previousShowTextureCoordinates
     || showNormals != previousShowNormals)
    {
        rebuildPanels();
    }

    previousShowVertices = showVertices;
    previousShowTextureCoordinates = showTextureCoordinates;
    previousShowNormals = showNormals;
}

void UIVertexInformation::rebuildPanels()
{
    components.clear();

    GLuint blankTexture = Texture::getTexture("Blank");

    float xpos = border;
    float buttonSize = 0.02f;
    addToggleButton(showVertices, xpos, buttonSize, blankTexture);
    addToggleButton(showTextureCoordinates, xpos, buttonSize, blankTexture);
    addToggleButton(showNormals, xpos, buttonSize, blankTexture);

    xpos = border;
    float ypos = border + textSize * 2;

    std::vector<glm::vec3>& vertices = infoSurface->getVertices();
    std::vector<glm::vec2>& textureCoordinates = infoSurface->getTextureCoordinates();
    std::vector<glm::vec3>& normals = infoSurface->getNormals();
    for (unsigned int i = 0; i<vertices.size(); i++)
    {
        xpos = border;

        addIndexLabel(i, glm::vec2(xpos, ypos));
        xpos += indexWidth + border;

        bool shouldAddHeader = i==0;

        if (showVertices)
        {
            addRowVec3(vertices[i], xpos, ypos, shouldAddHeader, "Vertices", 'X', blankTexture);
        }

        if (textureCoordinates.size() != 0
            && showTextureCoordinates)
        {
            addRowVec2(textureCoordinates[i], xpos, ypos, shouldAddHeader, "Texture Coordinates", 'U', blankTexture);
        }

        if (normals.size() != 0
            && showNormals)
        {
            addRowVec3(normals[i], xpos, ypos, shouldAddHeader, "Normals", 'X', blankTexture);
        }

        ypos += textSize + 0.002f;
    }

    setSize(glm::vec2(xpos, ypos + border - 0.002f));
    constructSurface();
    surface->diffuseMap = blankTexture;
}

void UIVertexInformation::addRowVec2(glm::vec2& row, float& xpos, float ypos, bool shouldAddHeader, std::string header, char firstSubHeader, GLuint texture)
{
    if (shouldAddHeader) addHeader(header, xpos, 2);
    for (unsigned int j = 0; j < 2; j++)
    {
        if (shouldAddHeader) addSubHeader(std::string(1, firstSubHeader + j), xpos);
        addNumber(row[j], glm::vec2(xpos, ypos), texture);
        xpos += columnWidth + border;
    }
}

void UIVertexInformation::addRowVec3(glm::vec3& row, float& xpos, float ypos, bool shouldAddHeader, std::string header, char firstSubHeader, GLuint texture)
{
    if (shouldAddHeader) addHeader(header, xpos, 3);
    for (unsigned int j = 0; j < 3; j++)
    {
        if (shouldAddHeader) addSubHeader(std::string(1, firstSubHeader + j), xpos);
        addNumber(row[j], glm::vec2(xpos, ypos), texture);
        xpos += columnWidth + border;
    }
}

void UIVertexInformation::addHeader(std::string text, float xPosition, int numColumns)
{
    float labelWidth = numColumns * (columnWidth + border) - border;

    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xPosition, border), glm::vec2(labelWidth, textSize));
    label->setText(text, textSize, *font, CENTRE);
    addComponent(label);
}

void UIVertexInformation::addSubHeader(std::string text, float xPosition)
{
    UILabel* label = new UILabel();
    label->setPositionAndSize(glm::vec2(xPosition, border + textSize), glm::vec2(columnWidth, textSize));
    label->setText(text, textSize, *font, CENTRE);
    addComponent(label);
}

void UIVertexInformation::addNumber(float& number, glm::vec2 position, GLuint background)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3) << number;
    std::string str = ss.str();

    UINumber* numberBox = new UINumber(number);
    numberBox->setPositionAndSize(position, glm::vec2(columnWidth, textSize));
    numberBox->setText(str, textSize, *font, RIGHT);
    numberBox->surface->diffuseMap = background;
    addComponent(numberBox);
}

void UIVertexInformation::addIndexLabel(unsigned int& number, glm::vec2 position)
{
    std::stringstream ss;
    ss << number;
    std::string str = ss.str();

    UILabel* label = new UILabel();
    label->setPositionAndSize(position, glm::vec2(indexWidth, textSize));
    label->setText(str, textSize, *font, RIGHT);
    addComponent(label);
}

void UIVertexInformation::addToggleButton(bool& toggle, float& xpos, float buttonSize, GLuint texture)
{
    UIToggleButton* button = new UIToggleButton(toggle);
    button->setPositionAndSize(glm::vec2(xpos, 0.01f), glm::vec2(buttonSize, buttonSize));
    button->surface->diffuseMap = texture;
    addComponent(button);

    xpos += buttonSize + border;
}