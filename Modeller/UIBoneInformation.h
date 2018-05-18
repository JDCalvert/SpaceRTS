#ifndef UI_BONE_INFORMATION_H
#define UI_BONE_INFORMATION_H

#include "UIInformation.h"
#include "UIBonePanel.h"

class UIBoneInformation : public UIInformation
{
public:
    float indexWidth;
    float columnWidth;

    UIBoneInformation(Surface* infoSurface);

    void build();
    void preRender() override;

    std::vector<UIBonePanel*>& getBonePanels();

protected:
    UIComponent* addPanel(unsigned int i);
    int getNumItemsTotal() override;

private:
    std::vector<UIBonePanel*> bonePanels;
};

#endif