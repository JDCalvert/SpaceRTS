#include "UIVertexInformation.h"

#include <sstream>
#include <iomanip>

#include <Font.h>
#include <Texture.h>

#include "UILabel.h"
#include "UINumber.h"

void UIVertexInformation::build(std::vector<glm::vec3>& vertices)
{
    float ypos = 0.01f;
    textSize = 0.025f;
    columnWidth = 0.10f;

    float xLabelPos = 0.01f;
    float yLabelPos = xLabelPos + columnWidth + 0.01f;
    float zLabelPos = yLabelPos + columnWidth + 0.01f;

    float panelWidth = zLabelPos + columnWidth + 0.01f;

    font = &Font::getFont("Calibri");
    GLuint blankTexture = Texture::getTexture("Blank");

    addHeader("x", xLabelPos);
    addHeader("y", yLabelPos);
    addHeader("z", zLabelPos);

    ypos += textSize;

    for (unsigned int i = 0; i<vertices.size(); i++)
    {
        glm::vec3& vertexPosition = vertices[i];

        addNumber(vertexPosition.x, glm::vec2(xLabelPos, ypos), blankTexture);
        addNumber(vertexPosition.y, glm::vec2(yLabelPos, ypos), blankTexture);
        addNumber(vertexPosition.z, glm::vec2(zLabelPos, ypos), blankTexture);

        ypos += textSize + 0.001f;
    }

    setSize(glm::vec2(panelWidth, ypos));
    constructSurface();
    surface->diffuseMap = blankTexture;
}

void UIVertexInformation::addHeader(std::string text, float xPosition)
{
    UILabel* xPosLabel = new UILabel();
    xPosLabel->setPositionAndSize(glm::vec2(xPosition, 0.01f), glm::vec2(columnWidth, textSize));
    xPosLabel->setText(text, textSize, *font, CENTRE);
    addComponent(xPosLabel);
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