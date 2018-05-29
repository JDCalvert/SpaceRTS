#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

class UIComponent;

class UILayout
{
public:
    virtual void layout(UIComponent* uiComponent) = 0;
};

#endif