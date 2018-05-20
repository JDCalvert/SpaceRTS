#ifndef UI_ACTION_LISTENER_H
#define UI_ACTION_LISTENER_H

class UIComponent;

class UIActionListener
{
public:
    virtual void actionPerformed(UIComponent* component) = 0;
};

#endif