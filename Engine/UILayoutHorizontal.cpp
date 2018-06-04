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

    std::vector<UIComponent*> components = component->components;
    for (auto i=components.begin(); i!=components.end(); i++)
    {
        UIComponent* childComponent = *i;

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
    //We want to increase their size proportionally, so work out their widths now
}