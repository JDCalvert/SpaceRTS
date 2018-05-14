#include "UITriangleInformation.h"

#include <Texture.h>
#include <UILabel.h>
#include <UIInteger.h>

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
        std::string

        UIInteger* uiInteger = new UIInteger(indices[i]);
        uiInteger->setPositionAndSize(glm::vec2(xpos, ypos), glm::vec2(indexWidth, ypos));
        uiInteger->setText(str, textSize, *font, RIGHT);
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