#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "UIComponent.h"

class UIButton : public UIComponent
{
public:
    bool toggle;

protected:
    void processMouseEvent(MouseEvent* mouseEvent);
};

#endif