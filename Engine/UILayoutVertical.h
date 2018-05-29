#ifndef UI_LAYOUT_VERTICAL_H
#define UI_LAYOUT_VERTICAL_H

#include "UILayout.h"

class UILayoutVertical: public UILayout
{
public:
    float externalHorizontalBorder;
    float externalVerticalBorder;
    float internalVerticalBorder;

    bool horizontalStretch;

    UILayoutVertical();

    void layout(UIComponent* uiComponent);
};

#endif