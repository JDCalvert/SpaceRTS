#ifndef UI_BONE_INFORMATION_H
#define UI_BONE_INFORMATION_H

#include "UIInformation.h"
#include "UIActionListener.h"
#include "UIBonePanel.h"

class UIBoneInformation : public UIInformation
{
public:
    float indexWidth;
    float columnWidth;
    float buttonSize;
    
    bool updateVertices = false;

    UIBoneInformation(Surface* infoSurface);

    void build() override;
    void preRender() override;

    std::vector<UIBonePanel*>& getBonePanels();

    void actionPerformed(UIComponent* component);

protected:
    UIComponent* addPanel(unsigned int i);
    int getNumItemsTotal() override;

private:
    std::vector<UIBonePanel*> bonePanels;
};

#endif