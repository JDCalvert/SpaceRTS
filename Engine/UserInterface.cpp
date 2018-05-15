#include "UserInterface.h"

#include "OpenGLContext.h"
#include "Event.h"
#include "Shader.h"

#include "UIComponent.h"
#include "UIShader.h"

UserInterface* UserInterface::instance;

UserInterface* UserInterface::getInstance()
{
    return UserInterface::instance;
}

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

void UserInterface::clearActiveComponent()
{
    if (activeComponent) activeComponent->processNotActive();
    activeComponent = nullptr;
}

void UserInterface::clearActiveComponent(UIComponent* component)
{
    if (activeComponent == component) clearActiveComponent();
}

bool UserInterface::hasActiveComponent()
{
    return activeComponent;
}

UIComponent* UserInterface::getActiveComponent()
{
    return activeComponent;
}

void UserInterface::handleEvents()
{
    glm::vec2 cursorPosition = OpenGLContext::currentContext()->getCursorPosition();
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;
        component->checkHover(cursorPosition);
    }

    Event* event = OpenGLContext::currentContext()->nextEvent();
    if (event == nullptr) return;

    if (event->type == MOUSE)
    {
        MouseClickEvent* mouseEvent = (MouseClickEvent*)event;
        if (mouseEvent->action == GLFW_PRESS)
        {
            //We might have an active component already. If we clicked on another component, we
            //should tell the active one that it's no longer active.
            UIComponent* eventComponent = nullptr;

            for (auto i = components.begin(); i != components.end() && !eventComponent; i++)
            {
                UIComponent* component = *i;
                eventComponent = component->checkAndProcessMouseEvent(mouseEvent);
            }

            if (activeComponent != eventComponent)
            {
                clearActiveComponent();
            }
            
            if (eventComponent
             && eventComponent->shouldRemainActive())
            {
                activeComponent = eventComponent;
            }
        }
    }
    else if (event->type == KEY)
    {
        if (activeComponent) activeComponent->processKeyEvent((KeyEvent*)event);
    }
    else if (event->type == TEXT)
    {
        if (activeComponent) activeComponent->processTextEvent((TextEvent*)event);
    }

    delete event;
}