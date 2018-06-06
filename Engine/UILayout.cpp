#include "UILayout.h"

UILayout::UILayout(UIComponent* component) :
    component(component)
{
    externalBorder = glm::vec2(0.01f);
    internalBorder = glm::vec2(0.002f);
}