#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "UIActionListener.h"

class UIButton : public UIComponent
{
public:
    UIButton(UIActionListener* listener);

protected:
    void processMouseEvent(MouseClickEvent* mouseEvent);

private:
    UIActionListener* listener;
};

#endif