#ifndef UI_PANEL_H
#define UI_PANEL_H

#include "UIComponent.h"

class UIPanel : public UIComponent
{
protected:
    void processMouseEvent(MouseEvent* mouseEvent);
};

#endif