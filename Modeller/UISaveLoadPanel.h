#ifndef UI_SAVE_LOAD_PANEL_H
#define UI_SAVE_LOAD_PANEL_H

#include <UIPanel.h>
#include <UIActionListener.h>

#include <Font.h>

class UIButton;
class UITextBox;

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
    void addBoxAndButton(UITextBox*& textBox, UIButton*& button, std::string buttonText, float& ypos);

    void actionPerformed(UIComponent* component);

private:
    UITextBox* loadBox;
    UITextBox* saveBox;
    UITextBox* importBox;
    UIButton* loadButton;
    UIButton* saveButton;
    UIButton* importButton;
};

#endif