#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include "UIComponent.h"

class UILayout
{
public:
    glm::vec2 externalBorder;
    glm::vec2 internalBorder;

    UILayout(UIComponent* component);

    virtual void layoutComponents() = 0;
    virtual void stretchComponents() = 0;

protected:
    UIComponent* component;
};

#endif