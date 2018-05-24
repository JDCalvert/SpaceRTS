#ifndef UI_TEXTURE_INFORMATION_H
#define UI_TEXTURE_INFORMATION_H

#include <UIToggleButton.h>

#include "UIInformation.h"
#include "UITexturePanel.h"

class UITextureInformation : public UIPanel
{
public:
    float textureSize;

    UITextureInformation(Surface* infoSurface);

    void build();
    void preRender() override;

private:
    Surface* infoSurface;

    bool showSpecular, previousShowSpecular;
    bool showNormal, previousShowNormal;
    UIToggleButton* showSpecularButton;
    UIToggleButton* showNormalButton;
    UITexturePanel* diffusePanel;
    UITexturePanel* specularPanel;
    UITexturePanel* normalPanel;

    void addTexturePanel(UITexturePanel*& panel, GLuint& textureId, std::string textureName);
    void addToggleButton(UIToggleButton*& toggleButton, bool& toggle, std::string buttonText);

    void rebuildPanel();
    void replaceTexturePanel(UITexturePanel* panel);

    float xpos, height;
    float buttonSize;
};

#endif