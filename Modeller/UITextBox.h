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

protected:
    UILabel* label;
};

#endif