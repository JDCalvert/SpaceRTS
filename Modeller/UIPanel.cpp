#include "UIPanel.h"

bool UIPanel::processMouseEvent(MouseEvent* mouseEvent)
{
    //Go through our sub-components and, check if any were clicked. If so process and then return out.
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;
        if (component->checkAndProcessMouseEvent(mouseEvent))
        {
            return true;
        }
    }

    //Even if none of the components were clicked, this panel was, so return true anyway
    return true;
}