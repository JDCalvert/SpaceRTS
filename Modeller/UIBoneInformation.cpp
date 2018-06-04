#include "UIBoneInformation.h"

#include <OpenGLContext.h>
#include <UIToggleButton.h>
#include <UILayoutVertical.h>
#include <UILayoutHorizontal.h>
#include <Texture.h>

#include "UserInterfaceModeller.h"
#include "UIBonePanel.h"

UIBoneInformation::UIBoneInformation(Surface* infoSurface) : UIInformation(infoSurface)
{
    indexWidth = 0.03f;
    columnWidth = 0.075f;
    buttonSize = 0.02f;

    newButtonXpos = border * 3 + indexWidth * 2;

    numItemsDisplay = 3;
}

void UIBoneInformation::build()
{
    bonePanels.clear();
    clearComponents();

    UILayoutVertical* verticalLayout = new UILayoutVertical(this);
    layout = verticalLayout;

    addButtons();
    addPanels();

    recalculateLayout();
}

void UIBoneInformation::addButtons()
{
    UIPanel* buttonPanel = new UIPanel();
    buttonPanel->setSize(0.1f, textSize);
    buttonPanel->surface.diffuseMap = Texture::getTexture("BlankNothing");
    addComponent(buttonPanel);

    UILayoutHorizontal* buttonLayout = new UILayoutHorizontal(buttonPanel);
    buttonLayout->externalHorizontalBorder = 0.0f;
    buttonLayout->externalVerticalBorder = 0.0f;
    buttonPanel->layout = buttonLayout;

    UIToggleButton* updateVerticesButton = new UIToggleButton(updateVertices);
    updateVerticesButton->setText("Update Vertices", buttonSize, *font, CENTRE);
    updateVerticesButton->setSize(0.1f, buttonSize);
    updateVerticesButton->surface.diffuseMap = texture;
    buttonPanel->addComponent(updateVerticesButton);

    UIToggleButton* updateTextureCoordinatesButton = new UIToggleButton(updateTextureCoordinates);
    updateTextureCoordinatesButton->setText("Update Texture Coordinates", buttonSize, *font, CENTRE);
    updateTextureCoordinatesButton->setSize(0.1f, buttonSize);
    updateTextureCoordinatesButton->surface.diffuseMap = texture;
    buttonPanel->addComponent(updateTextureCoordinatesButton);
}

void UIBoneInformation::preRender()
{
    setPosition(OpenGLContext::currentContext()->getAspectRatio() - (size.x + 0.01f), 1 - (size.y + 0.01f));

    if (shouldRebuild)
    {
        build();
        shouldRebuild = false;
    }
}

UIComponent* UIBoneInformation::addPanel(unsigned int i)
{
    UIBonePanel* bonePanel = new UIBonePanel(this, infoSurface, i);
    bonePanel->setPosition(border, ypos);
    bonePanel->buildPanel();
    addComponent(bonePanel);

    bonePanels.push_back(bonePanel);

    return bonePanel;
}

std::vector<UIBonePanel*>& UIBoneInformation::getBonePanels()
{
    return bonePanels;
}

int UIBoneInformation::getNumItemsTotal()
{
    return infoSurface->bones.size();
}

void UIBoneInformation::actionPerformed(UIComponent* component)
{
    if (component == newButton)
    {
        UserInterfaceModeller::getInstance()->newBone();

        currentItem = std::max(0, numItemsTotal - numItemsDisplay + 1);
        shouldRebuild = true;
    }
}