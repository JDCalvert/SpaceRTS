#include "UILayoutVertical.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutVertical::UILayoutVertical(UIComponent* component) : UILayout(component)
{
    externalHorizontalBorder = 0.01f;
    externalVerticalBorder = 0.01f;

    internalVerticalBorder = 0.002f;
}

void UILayoutVertical::layoutComponents()
{
    float xpos = externalHorizontalBorder;
    float ypos = externalVerticalBorder;

    widestComponent = 0.0f;

    //Layout all of the internal components according to their layouts
    std::vector<UIComponent*>& components = component->components;
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* childComponent = *i;

        UILayout* componentLayout = childComponent->layout;
        if (componentLayout) componentLayout->layoutComponents();

        childComponent->setPosition(xpos, ypos);

        glm::vec2 componentSize = childComponent->getSize();
        ypos += componentSize.y + internalVerticalBorder;
        widestComponent = std::max(widestComponent, componentSize.x);
    }

    component->setSize(
        widestComponent + 2 * externalHorizontalBorder,
        ypos + externalVerticalBorder - internalVerticalBorder
    );
}

void UILayoutVertical::stretchComponents()
{
    std::vector<UIComponent*>& components = component->components;
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;

        glm::vec2 componentSize = component->getSize();
        component->setSize(widestComponent, componentSize.y);

        UILayout* componentLayout = component->layout;
        if (componentLayout) componentLayout->stretchComponents();
    }
}