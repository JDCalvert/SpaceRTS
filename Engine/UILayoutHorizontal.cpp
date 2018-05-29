#include "UILayoutHorizontal.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutHorizontal::UILayoutHorizontal()
{
    externalHorizontalBorder = 0.01f;
    externalVerticalBorder = 0.01f;

    internalHorizontalBorder = 0.002f;
}

void UILayoutHorizontal::layout(UIComponent* uiComponent)
{
    float xpos = externalHorizontalBorder;
    float ypos = externalVerticalBorder;

    float tallestComponent = 0.0f;

    //Layout all of the internal components according to their layouts
    std::vector<UIComponent*> components = uiComponent->components;
    for (auto i=components.begin(); i!=components.end(); i++)
    {
        UIComponent* component = *i;

        //UILayout* componentLayout = component->layout;
        //if (componentLayout) componentLayout->layout(component);

        component->setPosition(xpos, ypos);
        
        glm::vec2 componentSize = component->getSize();

        xpos += componentSize.x + internalHorizontalBorder;

        tallestComponent = std::max(tallestComponent, componentSize.y);
    }

    uiComponent->setSize(
        xpos + externalHorizontalBorder - internalHorizontalBorder,
        tallestComponent + 2*externalVerticalBorder
    );
}