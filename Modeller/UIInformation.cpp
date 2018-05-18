#include "UIInformation.h"

#include <algorithm>

#include <Texture.h>

UIInformation::UIInformation(Surface* infoSurface)
{
    this->infoSurface = infoSurface;

    border = 0.01f;
    textSize = 0.025f;

    currentItem = 0;

    font = &Font::getFont("Calibri");
    texture = Texture::getTexture("Blank");
}

void UIInformation::addPanels()
{
    float width = 0.0f;

    numItemsTotal = getNumItemsTotal();
    int maxItem = std::min(currentItem + numItemsDisplay, numItemsTotal);
    for (unsigned int i=currentItem; i<maxItem; i++)
    {
        UIComponent* panel = addPanel(i);

        glm::vec2 panelSize = panel->getSize();
        ypos += panelSize.y + 0.002f;
        width = std::max(width, panelSize.x);
    }

    setSize(width + border * 2, ypos + border - 0.002f);
    constructSurface();
    surface->diffuseMap = texture;
}

EventStatus UIInformation::processMouseScroll(MouseScrollEvent mouseEvent)
{
    if (mouseEvent.yOffset > 0
        && currentItem > 0)
    {
        currentItem--;
        build();
    }
    else if (mouseEvent.yOffset < 0
        && currentItem  < numItemsTotal - numItemsDisplay)
    {
        currentItem++;
        build();
    }

    return PROCESSED;
}