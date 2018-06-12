#include "UILayoutHorizontal.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutHorizontal::UILayoutHorizontal(UIComponent* component) : UILayout(component)
{
}

void UILayoutHorizontal::layoutComponents()
{
    glm::vec2 pos = externalBorder;
    
    float tallestComponent = 0.0f;

    for (UIComponent* childComponent : component->components)
    {
        childComponent->layoutComponents();
        childComponent->setPosition(pos);
        
        glm::vec2 componentSize = childComponent->getSize();
        pos.x += componentSize.x + internalBorder.x;

        tallestComponent = std::max(tallestComponent, componentSize.y);
    }
    pos.y += tallestComponent + internalBorder.y;

    component->setSize(pos + externalBorder - internalBorder);
}

void UILayoutHorizontal::stretchComponents()
{
    std::vector<UIComponent*>& components = component->components;

    float idealWidth = component->getSize().x;
    idealWidth -= (components.size() - 1) * internalBorder.x;
    idealWidth -= externalBorder.x * 2;

    //We want to increase their size proportionally, so work out their widths now.
    float totalWidth = 0.0f;
    for (UIComponent* component : components)
    {
        totalWidth += component->getSize().x;
    }

    glm::vec2 pos = externalBorder;

    float ratio = idealWidth / totalWidth;
    for (UIComponent* component : components)
    {
        glm::vec2 size = component->getSize();
        size.x *= ratio;

        component->setPosition(pos);
        component->setSize(size);

        pos.x += size.x + internalBorder.x;

        component->stretchComponents();
    }
}