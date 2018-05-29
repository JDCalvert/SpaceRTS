#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

class UIComponent;

class UILayout
{
public:
    UILayout(UIComponent* component);

    virtual void layoutComponents() = 0;
    virtual void stretchComponents() = 0;

protected:
    UIComponent* component;
};

#endif