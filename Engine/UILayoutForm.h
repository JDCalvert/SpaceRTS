#ifndef UI_LAYOUT_FORM_H
#define UI_LAYOUT_FORM_H

#include "UILayout.h"

class UILayoutForm : public UILayout
{
public:
    unsigned int groupSize;

    std::vector<float> largestInGroup;
    std::vector<float> largestInIndex;

    UILayoutForm(UIComponent* component, unsigned int groupSize);

    virtual int getComponentForGroup() = 0;
    virtual int getComponentForIndex() = 0;

    void layoutComponents() override;
    void stretchComponents() override;

    glm::vec2 calculateComponentsSize();
    int calculateNumGroups();
};

#endif