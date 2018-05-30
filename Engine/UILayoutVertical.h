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

    float widestComponent;

    UILayoutVertical(UIComponent* component);

    void layoutComponents() override;
    void stretchComponents() override;
};

#endif