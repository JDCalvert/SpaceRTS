#ifndef UI_SAVE_LOAD_PANEL_H
#define UI_SAVE_LOAD_PANEL_H

#include <UIPanel.h>
#include <UIActionListener.h>

#include <Font.h>

class UIButton;

class UISaveLoadPanel : public UIPanel, UIActionListener
{
public:
    Font* font;
    float textSize;
    float textBoxWidth;
    float buttonWidth;
    float border;

    GLuint blankTexture;

    void build();
    void preRender();

    void actionPerformed(UIComponent* component);

private:
    UIButton* loadButton;
    UIButton* saveButton;
};

#endif