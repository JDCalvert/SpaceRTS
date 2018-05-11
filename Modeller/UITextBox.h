#ifndef UI_TEXT_BOX_H
#define UI_TEXT_BOX_H

#include "UIPanel.h"
#include "UILabel.h"

class UITextBox : public UIPanel
{
public:
    UITextBox();

    void setText(std::string text, float size, Font font, Alignment alignment);
    void setSize(glm::vec2 size);

    void processKeyEvent(KeyEvent* event);
    void processTextEvent(TextEvent* event);

    bool shouldRemainActive();

protected:
    UILabel* label;

    bool shouldCheckMouseEventForChildren();
};

#endif