#include "UIBoneInformation.h"

#include <OpenGLContext.h>
#include <UIToggleButton.h>

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

    UIToggleButton* button = new UIToggleButton(updateVertices);
    button->setPosition(border, border);
    button->setSize(buttonSize, buttonSize);
    button->surface->diffuseMap = texture;
    addComponent(button);

    ypos = buttonSize + border * 2;

    addPanels();
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
    return infoSurface->getBones().size();
}

void UIBoneInformation::actionPerformed(UIComponent* component)
{
    if (std::find(bonePanels.begin(), bonePanels.end(), component) != bonePanels.end())
    {
        UIBonePanel* panel = static_cast<UIBonePanel*>(component);
        UserInterfaceModeller::getInstance()->recalculateVertexPositions();
    }
    else if (component == newButton)
    {
        std::vector<Bone>& bones = infoSurface->getBones();
        bones.push_back(Bone(glm::mat4(), -1));

        infoSurface->prepareBones();
        shouldRebuild = true;
    }
}