#ifndef UI_TEXT_BOX_H
#define UI_TEXT_BOX_H

#include "UIPanel.h"
#include "UILabel.h"
#include "UIActionListener.h"

class UITextBox : public UIPanel
{
public:
    UITextBox();

    void setText(std::string text, float size, Font font, Alignment alignment);
    void setSize(glm::vec2 size);

    void processKeyEvent(KeyEvent event) override;
    void processTextEvent(TextEvent event) override;

    bool shouldRemainActive() override;

    std::string getText();

protected:
    UILabel* label;

    bool shouldCheckMouseClickEventForChildren() override;
};

#endif