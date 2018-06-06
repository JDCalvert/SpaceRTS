#include "UILayoutVertical.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutVertical::UILayoutVertical(UIComponent* component) : UILayout(component)
{
}

void UILayoutVertical::layoutComponents()
{
    glm::vec2 pos = externalBorder;

    widestComponent = 0.0f;

    //Layout all of the internal components according to their layouts
    std::vector<UIComponent*>& components = component->components;
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* childComponent = *i;

        UILayout* componentLayout = childComponent->layout;
        if (componentLayout) componentLayout->layoutComponents();

        childComponent->setPosition(pos);

        glm::vec2 componentSize = childComponent->getSize();
        pos.y += componentSize.y + internalBorder.y;
        widestComponent = std::max(widestComponent, componentSize.x);
    }
    pos.x += widestComponent + internalBorder.x;

    component->setSize(pos + externalBorder - internalBorder);
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