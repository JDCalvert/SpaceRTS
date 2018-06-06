#ifndef UI_LAYOUT_FORM_HORIZONTAL_H
#define UI_LAYOUT_FORM_HORIZONTAL_H

#include "UILayoutForm.h"

class UILayoutFormHorizontal : public UILayoutForm
{
public:
    UILayoutFormHorizontal(UIComponent* component, unsigned int rowWidth);

    int getComponentForGroup() override;
    int getComponentForIndex() override;
};

#endif