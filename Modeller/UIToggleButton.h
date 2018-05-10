#ifndef UI_TOGGLE_BUTTON_H
#define UI_TOGGLE_BUTTON_H

#include "UIComponent.h"

class UIToggleButton : public UIComponent
{
public:
    UIToggleButton(bool& toggle);

protected:
    void processMouseEvent(MouseEvent* mouseEvent);
    bool& toggle;
};

#endif