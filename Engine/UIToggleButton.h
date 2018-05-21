#ifndef UI_TOGGLE_BUTTON_H
#define UI_TOGGLE_BUTTON_H

#include "UIComponent.h"
#include "UILabel.h"

class UIToggleButton : public UIComponent
{
public:
    UIToggleButton(bool& toggle);

    void setText(std::string text, float size, Font font, Alignment alignment);

    void preRender() override;

    using UIComponent::setSize;
    void setSize(glm::vec2 size) override;

protected:
    void processMouseClick(MouseClickEvent mouseEvent) override;
    bool shouldCheckMouseClickEventForChildren() override;

private:
    bool& toggle;
    GLuint onMap, offMap;

    UILabel* label;
};

#endif