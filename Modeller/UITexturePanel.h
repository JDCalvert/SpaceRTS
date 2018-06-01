#ifndef UI_TEXTURE_PANEL_H
#define UI_TEXTURE_PANEL_H

#include <UIPanel.h>
#include <UITextBox.h>
#include <UIButton.h>

class UITextureInformation;

class UITexturePanel : public UIPanel, UIActionListener
{
public:
    const std::string texturePrefix = "../Resources/Graphics/";
    const std::string textureSuffix = ".dds";

    float loadButtonWidth;

    GLuint& textureId;
    std::string& textureName;

    GLuint textureCoordinateId;    

    bool shouldRebuild;

    UITexturePanel(UITextureInformation* parent, GLuint& textureId, std::string& textureName, GLuint textureCoordinateId, std::string label);

    void build();
    void preRender() override;

    void actionPerformed(UIComponent* component) override;

    void updateTexture();

private:
    UITextureInformation* parent;

    UITextBox* textureNameBox;
    UIButton* textureLoadButton;
    UIPanel* texturePanel;
    UIPanel* textureCoordinatePanel;

    float textSize;
    float border;
    Font* font;
    GLuint texture;

    std::string labelText;

    std::string getShortTextureName();
};

#endif