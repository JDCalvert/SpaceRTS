#include "UIBoneInformation.h"

#include <OpenGLContext.h>

#include "UserInterfaceModeller.h"
#include "UIBonePanel.h"

UIBoneInformation::UIBoneInformation(Surface* infoSurface) : UIInformation(infoSurface)
{
    indexWidth = 0.03f;
    columnWidth = 0.075f;

    numItemsDisplay = 3;
}

void UIBoneInformation::build()
{
    setPosition(0.01f, 0.14f);

    bonePanels.clear();
    clearComponents();

    ypos = border;

    addPanels();
}

void UIBoneInformation::preRender()
{
    setPosition(OpenGLContext::currentContext()->getAspectRatio() - (size.x + 0.01f), 1 - (size.y + 0.01f));
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
}