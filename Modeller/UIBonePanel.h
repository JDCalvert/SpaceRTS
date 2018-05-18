#ifndef UI_BONE_PANEL_H
#define UI_BONE_PANEL_H

#include <UIPanel.h>

class UIBoneInformation;

class UIBonePanel : public UIPanel
{
public:
    UIBonePanel(UIBoneInformation* parent, Surface* infoSurface, unsigned int index);

    void buildPanel();
    void preRender() override;

    bool isHighlighted();

private:
    UIBoneInformation* parent;
    BindBone& bone;
    unsigned int index;

    float xpos, ypos;
    bool highlighted;
    GLuint onMap, offMap;

    void addIndexLabel();
    void addParentNumber();
    void addNumber(float& value);
};

#endif