#include "UILayoutFormHorizontal.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "UIComponent.h"

UILayoutFormHorizontal::UILayoutFormHorizontal(UIComponent* component, unsigned int rowWidth) : UILayoutForm(component, rowWidth)
{
}

int UILayoutFormHorizontal::getComponentForGroup()
{
    return 0;
}

int UILayoutFormHorizontal::getComponentForIndex()
{
    return 1;
}