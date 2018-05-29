#ifndef UI_LAYOUT_HORIZONTAL_H
#define UI_LAYOUT_HORIZONTAL_H

#include "UILayout.h"

class UILayoutHorizontal : public UILayout
{
public:
    float externalHorizontalBorder;
    float externalVerticalBorder;
    float internalHorizontalBorder;

    UILayoutHorizontal();

    void layout(UIComponent* uiComponent);
};

#endif