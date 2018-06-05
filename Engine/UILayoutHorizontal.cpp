#include "UILayoutHorizontal.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutHorizontal::UILayoutHorizontal(UIComponent* component) : UILayout(component)
{
    externalHorizontalBorder = 0.01f;
    externalVerticalBorder = 0.01f;

    internalHorizontalBorder = 0.002f;
}

void UILayoutHorizontal::layoutComponents()
{
    float xpos = externalHorizontalBorder;
    float ypos = externalVerticalBorder;

    float tallestComponent = 0.0f;

    for (UIComponent* childComponent : component->components)
    {
        UILayout* componentLayout = childComponent->layout;
        if (componentLayout) componentLayout->layoutComponents();

        childComponent->setPosition(xpos, ypos);
        
        glm::vec2 componentSize = childComponent->getSize();

        xpos += componentSize.x + internalHorizontalBorder;

        tallestComponent = std::max(tallestComponent, componentSize.y);
    }

    component->setSize(
        xpos + externalHorizontalBorder - internalHorizontalBorder,
        tallestComponent + 2 * externalVerticalBorder
    );
}

void UILayoutHorizontal::stretchComponents()
{
    std::vector<UIComponent*>& components = component->components;

    float idealWidth = component->getSize().x;
    idealWidth -= (components.size() - 1) * internalHorizontalBorder;
    idealWidth -= externalHorizontalBorder * 2;

    //We want to increase their size proportionally, so work out their widths now.
    float totalWidth = 0.0f;
    for (UIComponent* component : components)
    {
        totalWidth += component->getSize().x;
    }

    float xpos = externalHorizontalBorder;

    float ratio = idealWidth / totalWidth;
    for (UIComponent* component : components)
    {
        glm::vec2 size = component->getSize();
        size.x *= ratio;

        component->setPosition(xpos, externalVerticalBorder);
        component->setSize(size);

        xpos += size.x + internalHorizontalBorder;
    }
}