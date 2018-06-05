#include "UILayout.h"

UILayout::UILayout(UIComponent* component) :
    component(component)
{
    externalHorizontalBorder = 0.01f;
    externalVerticalBorder = 0.01f;

    internalHorizontalBorder = 0.002f;
    internalVerticalBorder = 0.002f;
}