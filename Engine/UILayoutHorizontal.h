#ifndef UI_LAYOUT_HORIZONTAL_H
#define UI_LAYOUT_HORIZONTAL_H

#include "UILayout.h"

class UILayoutHorizontal : public UILayout
{
public:
    UILayoutHorizontal(UIComponent* component);

    void layoutComponents();
    void stretchComponents();
};

#endif