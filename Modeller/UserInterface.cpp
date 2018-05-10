#include "UserInterface.h"

#include <OpenGLContext.h>
#include <Event.h>
#include <Shader.h>

#include "UIComponent.h"
#include "UIShader.h"

void UserInterface::render()
{
    UIShader* shader = (UIShader*)Shader::getShader("UI");

    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;
        shader->renderUiComponent(component);
    }
}

void UserInterface::addComponent(UIComponent* component)
{
    components.push_back(component);
}

void UserInterface::handleEvents()
{
    Event* event = OpenGLContext::currentContext()->nextEvent();
    if (event == nullptr) return;

    if (event->type == MOUSE)
    {
        MouseEvent* mouseEvent = (MouseEvent*)event;
        if (mouseEvent->action == GLFW_PRESS)
        {
            bool handled = false;
            for (auto i = components.begin(); i != components.end() && !handled; i++)
            {
                UIComponent* component = *i;
                component->checkAndProcessMouseEvent(mouseEvent);
            }
        }
    }
    else if (event->type == TEXT)
    {
        KeyEvent* keyEvent = (KeyEvent*)event;
    }

    delete event;
}