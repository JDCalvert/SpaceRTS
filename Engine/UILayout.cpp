#include "UILayout.h"

UILayout::UILayout(UIComponent* component) :
    component(component)
{
    externalBorder = glm::vec2(0.0f);
    internalBorder = glm::vec2(0.002f);
}