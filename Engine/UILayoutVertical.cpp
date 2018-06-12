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
    for (UIComponent* childComponent : component->components)
    {
        childComponent->layoutComponents();
        childComponent->setPosition(pos);

        glm::vec2 componentSize = childComponent->getSize();
        pos.y += componentSize.y + internalBorder.y;
        widestComponent = std::max(widestComponent, componentSize.x);
    }

    component->setSize(widestComponent + externalBorder.x * 2, pos.y + externalBorder.y - internalBorder.y);
}

void UILayoutVertical::stretchComponents()
{
    //Stretch all the subcomponents to be as wide as this
    glm::vec2 idealComponentsSize = component->getSize() - (externalBorder * 2.0f);

    for (UIComponent* childComponent : component->components)
    {
        glm::vec2 componentSize = childComponent->getSize();
        childComponent->setSize(idealComponentsSize.x, componentSize.y);
        childComponent->stretchComponents();
    }
}