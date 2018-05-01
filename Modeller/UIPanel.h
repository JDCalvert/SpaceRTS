#ifndef UI_PANEL_H
#define UI_PANEL_H

#include <glfw\glfw3.h>
#include <glm\glm.hpp>

#include <vector>

#include "MouseEvent.h"

#include "UIComponent.h"

class UIPanel : public UIComponent
{
public:
    void addComponent(UIComponent* component);

protected:
    bool processMouseEvent(MouseEvent* mouseEvent);

private:
    std::vector<UIComponent*> components;
};

#endif