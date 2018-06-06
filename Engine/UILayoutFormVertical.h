#ifndef UI_LAYOUT_FORM_VERTICAL
#define UI_LAYOUT_FORM_VERTICAL

#include "UILayoutForm.h"

class UILayoutFormVertical : public UILayoutForm
{
public:
    UILayoutFormVertical(UIComponent* component, unsigned int columnHeight);

    int getComponentForGroup() override;
    int getComponentForIndex() override;
};

#endif