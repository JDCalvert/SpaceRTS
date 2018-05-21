#include "UIInformation.h"

#include <algorithm>

#include <Texture.h>

UIInformation::UIInformation(Surface* infoSurface)
{
    this->infoSurface = infoSurface;

    border = 0.01f;
    textSize = 0.025f;
    newButtonWidth = 0.05f;
    newButtonXpos = border;

    currentItem = 0;

    shouldRebuild = false;

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

    addNewButton();

    setSize(width + border * 2, ypos);
    surface->diffuseMap = texture;
}

void UIInformation::addNewButton()
{
    newButton = new UIButton(this);
    newButton->setPosition(newButtonXpos, ypos);
    newButton->setSize(newButtonWidth, textSize);
    newButton->setText("New", textSize, *font, CENTRE);
    newButton->surface->diffuseMap = texture;
    addComponent(newButton);

    ypos += textSize + border;
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