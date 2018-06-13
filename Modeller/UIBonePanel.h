#ifndef UI_BONE_PANEL_H
#define UI_BONE_PANEL_H

#include <UIPanel.h>
#include <UIActionListener.h>

class UIBoneInformation;
class UIButton;

class UIBonePanel : public UIPanel, UIActionListener
{
public:
    UIBonePanel(UIBoneInformation* parent, Surface* infoSurface, unsigned int index);

    glm::vec2 textureCoordinateTopLeft;
    glm::vec2 textureCoordinateBottomRight;

    void buildPanel();
    void preRender() override;

    bool isHighlighted();

    unsigned int getIndex();

    void actionPerformed(UIComponent* component) override;

protected:
    void processMouseClick(MouseClickEvent mouseEvent) override;

private:
    UIBoneInformation* parent;

    UIButton* removeButton;

    Bone& bone;
    unsigned int index;

    bool highlighted;
    GLuint onMap, offMap;

    void addIndexPanel();
    void addNumber(float& value);
    void addRemoveButton();

    void addTextureCoordinatePanel(glm::vec2& textureCoordinate);
};

#endif