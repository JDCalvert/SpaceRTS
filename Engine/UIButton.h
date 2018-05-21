#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "UIActionListener.h"
#include "UIComponent.h"
#include "UILabel.h"

class UIButton : public UIComponent
{
public:
    UIButton(UIActionListener* listener);

    void setText(std::string text, float size, Font font, Alignment alignment);

    using UIComponent::setSize;
    void setSize(glm::vec2 size) override;

protected:
    void processMouseClick(MouseClickEvent mouseEvent) override;
    bool shouldCheckMouseClickEventForChildren() override;

private:
    UIActionListener* listener;
    UILabel* label;
};

#endif