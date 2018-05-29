#ifndef UI_IMPORT_PANEL_H
#define UI_IMPORT_PANEL_H

#include <Font.h>
#include <UIPanel.h>
#include <UIButton.h>
#include <UITextBox.h>
#include <UIActionListener.h>

class UIImportPanel : public UIPanel, public UIActionListener
{
public:
    glm::vec2 textureCoordinatesTopLeft;
    glm::vec2 textureCoordinatesBottomRight;

    void build();
    void actionPerformed(UIComponent* component);

private:
    Font* font;
    float textSize;
    float border;
    float width;

    GLuint blankTexture;

    UITextBox* fileTextBox;
    UIButton* importButton;

    void addHeader(std::string text);
    void addTextBox(UITextBox*& textBox);
    void addNumber(UIPanel* panel, float& number);
};

#endif