#ifndef UI_TOGGLE_BUTTON_H
#define UI_TOGGLE_BUTTON_H

#include "UIComponent.h"

class UIToggleButton : public UIComponent
{
public:
    UIToggleButton(bool& toggle);

    void preRender();

protected:
    void processMouseEvent(MouseClickEvent* mouseEvent);
    bool& toggle;

    GLuint onMap, offMap;
};

#endif