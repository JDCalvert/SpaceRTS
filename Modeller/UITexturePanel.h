#ifndef UI_TEXTURE_PANEL_H
#define UI_TEXTURE_PANEL_H

#include <UIPanel.h>
#include <UITextBox.h>
#include <UIButton.h>

class UITextureInformation;

class UITexturePanel : public UIPanel, UIActionListener
{
public:
    float loadButtonWidth;

    GLuint& textureId;

    bool shouldRebuild;

    UITexturePanel(UITextureInformation* parent, GLuint& textureId, std::string label);

    void build();
    void preRender() override;

    void actionPerformed(UIComponent* component);

protected:

private:
    UITextureInformation* parent;

    UITextBox* diffuseTextureBox;
    UIButton* diffuseTextureLoadButton;
    UIPanel* texturePanel;

    float textSize;
    float border;
    Font* font;
    GLuint texture;

    std::string labelText;
};

#endif