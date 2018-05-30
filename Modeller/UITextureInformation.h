#ifndef UI_TEXTURE_INFORMATION_H
#define UI_TEXTURE_INFORMATION_H

#include <UIToggleButton.h>

#include "UIInformation.h"
#include "UITexturePanel.h"
#include "TextureRenderer.h"

class UITextureInformation : public UIPanel
{
public:
    float textureSize;

    UITextureInformation(Surface* infoSurface);

    void build();
    void preRender() override;

    void renderTextureCoordinates();
    void rebuildImportTextureCoordinates();

private:
    Surface* infoSurface;

    TextureRenderer* textureCoordinateRenderer;

    float xpos, height;
    float buttonSize;

    bool showSpecular, previousShowSpecular;
    bool showNormal, previousShowNormal;
    UIToggleButton* showSpecularButton;
    UIToggleButton* showNormalButton;
    UITexturePanel* diffusePanel;
    UITexturePanel* specularPanel;
    UITexturePanel* normalPanel;

    std::vector<glm::vec3> importVertices;
    std::vector<unsigned int> importIndices;

    void addTexturePanel(UITexturePanel*& panel, GLuint& textureId, GLuint textureCoordinateTextureId, std::string textureName);
    void addToggleButton(UIToggleButton*& toggleButton, bool& toggle, std::string buttonText);

    void rebuildPanel();
    void replaceTexturePanel(UITexturePanel* panel);
};

#endif