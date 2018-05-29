#include "UILayoutVertical.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutVertical::UILayoutVertical(UIComponent* component) : UILayout(component)
{
    externalHorizontalBorder = 0.01f;
    externalVerticalBorder = 0.01f;

    internalVerticalBorder = 0.002f;
}

void UILayoutVertical::layout()
{
    float xpos = externalHorizontalBorder;
    float ypos = externalVerticalBorder;

    float widestComponent = 0.0f;

    //Layout all of the internal components according to their layouts
    std::vector<UIComponent*> components = uiComponent->components;
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;

        UILayout* componentLayout = component->layout;
        if (componentLayout) componentLayout->layout(component);

        component->setPosition(xpos, ypos);

        glm::vec2 componentSize = component->getSize();

        ypos += componentSize.y + internalVerticalBorder;

        widestComponent = std::max(widestComponent, componentSize.x);
    }

    uiComponent->setSize(
        widestComponent + 2 * externalHorizontalBorder,
        ypos + externalVerticalBorder - internalVerticalBorder
    );

    //Now we've laid everything out, stretch the components to fill the area
    if (horizontalStretch)
    {
        for (auto i = components.begin(); i != components.end(); i++)
        {
            UIComponent* component = *i;

            glm::vec2 componentSize = component->getSize();
            component->setSize(widestComponent, componentSize.y);

            //UILayout* componentLayout = component->layout;
            //if (componentLayout) componentLayout->stretch(component);
        }
    }
}