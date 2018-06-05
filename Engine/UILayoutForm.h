#ifndef UI_LAYOUT_FORM_H
#define UI_LAYOUT_FORM_H

#include "UILayout.h"

class UILayoutForm : public UILayout
{
public:
    int rowWidth;

    UILayoutForm(UIComponent* component);

    void layoutComponents() override;
    void stretchComponents() override;
};

#endif