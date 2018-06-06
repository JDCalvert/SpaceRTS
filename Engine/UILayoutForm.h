#ifndef UI_LAYOUT_FORM_H
#define UI_LAYOUT_FORM_H

#include "UILayout.h"

class UILayoutForm : public UILayout
{
public:
    UILayoutForm(UIComponent* component, unsigned int groupSize);

    unsigned int groupSize;

    virtual int getComponentForGroup() = 0;
    virtual int getComponentForIndex() = 0;

    void layoutComponents() override;
    void stretchComponents() override;
};

#endif