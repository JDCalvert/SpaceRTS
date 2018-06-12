#include "UIBoneInformation.h"

#include <OpenGLContext.h>
#include <UIToggleButton.h>
#include <UILayoutVertical.h>
#include <UILayoutHorizontal.h>
#include <UILayoutFormHorizontal.h>
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

    layout = new UILayoutVertical(this);
    layout->externalBorder = glm::vec2(0.01f, 0.01f);

    addTopPanel();
    addPanels();

    recalculateLayout();
}

void UIBoneInformation::addTopPanel()
{
    UIPanel* buttonPanel = new UIPanel();
    buttonPanel->setSize(0.1f, textSize);
    buttonPanel->layout = new UILayoutFormHorizontal(buttonPanel, 2);
    addComponent(buttonPanel);

    UILabel* verticesLabel = new UILabel();
    verticesLabel->setText("Vertices", buttonSize, *font, LEFT);
    verticesLabel->setSize(0.1f, buttonSize);
    buttonPanel->addComponent(verticesLabel);

    UIPanel* verticesButtons = new UIPanel();
    verticesButtons->layout = new UILayoutHorizontal(verticesButtons);
    buttonPanel->addComponent(verticesButtons);

    UIToggleButton* updateVerticesButton = new UIToggleButton(updateVertices);
    updateVerticesButton->setText("Update", buttonSize, *font, CENTRE);
    updateVerticesButton->setSize(0.1f, buttonSize);
    updateVerticesButton->surface.diffuseMap = texture;
    verticesButtons->addComponent(updateVerticesButton);

    UILabel* textureCoordinatesLabel = new UILabel();
    textureCoordinatesLabel->setText("Texture Coordinates", buttonSize, *font, LEFT);
    textureCoordinatesLabel->setSizeFromText();
    buttonPanel->addComponent(textureCoordinatesLabel);

    UIPanel* textureCoordinateButtons = new UIPanel();
    textureCoordinateButtons->layout = new UILayoutHorizontal(textureCoordinateButtons);
    buttonPanel->addComponent(textureCoordinateButtons);

    UIToggleButton* showTextureCoordinatesButton = new UIToggleButton(showTextureCoordinates);
    showTextureCoordinatesButton->setText("Show", buttonSize, *font, CENTRE);
    showTextureCoordinatesButton->setSize(0.1f, buttonSize);
    showTextureCoordinatesButton->surface.diffuseMap = texture;
    textureCoordinateButtons->addComponent(showTextureCoordinatesButton);

    UIToggleButton* updateTextureCoordinatesButton = new UIToggleButton(updateTextureCoordinates);
    updateTextureCoordinatesButton->setText("Update", buttonSize, *font, CENTRE);
    updateTextureCoordinatesButton->setSize(0.1f, buttonSize);
    updateTextureCoordinatesButton->surface.diffuseMap = texture;
    textureCoordinateButtons->addComponent(updateTextureCoordinatesButton);
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