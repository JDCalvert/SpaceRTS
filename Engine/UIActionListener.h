#ifndef UI_ACTION_LISTENER_H
#define UI_ACTION_LISTENER_H

#include "UIComponent.h"

class UIActionListener
{
public:
    virtual void actionPerformed(UIComponent* component) = 0;
};

#endif