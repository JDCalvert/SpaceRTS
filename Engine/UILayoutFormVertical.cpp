#include "UILayoutFormVertical.h"

#include "UIComponent.h"

UILayoutFormVertical::UILayoutFormVertical(UIComponent* component, unsigned int columnHeight) : UILayoutForm(component, columnHeight)
{
}

int UILayoutFormVertical::getComponentForGroup()
{
    return 1;
}

int UILayoutFormVertical::getComponentForIndex()
{
    return 0;
}