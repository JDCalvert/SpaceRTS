#include "UIComponent.h"

#include "Geometry.h"
#include "UserInterface.h"

UIComponent::~UIComponent()
{
    clearComponents();
}

UIComponent* UIComponent::checkAndProcessMouseClickEvent(MouseClickEvent mouseEvent)
{
    //If this wasn't clicked, then none of the subcomponents will have been.
    if (!isPointOnMe(mouseEvent.position)) return nullptr;

    if (shouldCheckMouseClickEventForChildren())
    {
        MouseClickEvent relativeMouseEvent = mouseEvent.getRelative(position);

        //Go through our sub-components and, check if any were clicked. If so process and then return that component.
        for (auto i = components.begin(); i != components.end(); i++)
        {
            UIComponent* component = *i;
            UIComponent* eventComponent = component->checkAndProcessMouseClickEvent(relativeMouseEvent);
        
            if (eventComponent)
            {
                return eventComponent;
            }
        }
    }

    //If none of our child components were clicked, then this was clicked
    processMouseClick(mouseEvent);
    return this;
}

EventStatus UIComponent::checkAndProcessMouseScrollEvent(MouseScrollEvent mouseEvent)
{
    //If we aren't hovering over this, then don't do anything with this or any of its children
    if (!isPointOnMe(mouseEvent.position)) return NOT_HANDLED;

    MouseScrollEvent relativeMouseEvent = mouseEvent.getRelative(position);
    
    //Go through the children and process them
    EventStatus status = NOT_HANDLED;
    for (auto i = components.begin(); i != components.end() && status == NOT_HANDLED; i++)
    {
        UIComponent* component = *i;
        EventStatus status = component->checkAndProcessMouseScrollEvent(relativeMouseEvent);
        if (status == PROCESSED)
        {
            return PROCESSED;
        }
    }

    //If none of our children processed the scroll event, then return our own
    return processMouseScroll(mouseEvent);
}

bool UIComponent::isPointOnMe(glm::vec2 point)
{
    return Geometry::isPointInBox(point, position, size);
}

void UIComponent::checkHover(glm::vec2 mousePosition)
{
    if (isPointOnMe(mousePosition))
    {
        hover = true;
        for (auto i = components.begin(); i != components.end(); i++)
        {
            UIComponent* component = *i;
            component->checkHover(mousePosition - position);
        }
    }
    else
    {
        setNoHover();
    }
}

void UIComponent::setNoHover()
{
    hover = false;
    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* component = *i;
        component->setNoHover();
    }
}

void UIComponent::processMouseClick(MouseClickEvent mouseEvent)
{
}

EventStatus UIComponent::processMouseScroll(MouseScrollEvent mouseEvent)
{
    return HANDLED_NOT_PROCESSED;
}

void UIComponent::processKeyEvent(KeyEvent keyEvent)
{
}

void UIComponent::processTextEvent(TextEvent textEvent)
{
}

void UIComponent::becomeInactive()
{
    UserInterface* ui = UserInterface::getInstance();
    ui->clearActiveComponent(this);

    if (actionListener) actionListener->actionPerformed(this);
}

bool UIComponent::isChild(UIComponent* component)
{
    if (!component) return false;

    for (auto i = components.begin(); i != components.end(); i++)
    {
        UIComponent* childComponent = *i;
        if (childComponent == component
          || childComponent->isChild(component))
        {
            return true;
        }
    }

    return false;
}

void UIComponent::processNotActive()
{
}

bool UIComponent::shouldRemainActive()
{
    return false;
}

void UIComponent::setActionListener(UIActionListener* actionListener)
{
    this->actionListener = actionListener;
}

void UIComponent::invokeActionListener()
{
    if (actionListener) actionListener->actionPerformed(this);
}

void UIComponent::addComponent(UIComponent* component)
{
    components.push_back(component);
}

void UIComponent::setPosition(float x, float y)
{
    setPosition(glm::vec2(x, y));
}

void UIComponent::setPosition(glm::vec2 position)
{
    this->position = position;
}

void UIComponent::setSize(float x, float y)
{
    setSize(glm::vec2(x, y));
}

void UIComponent::setSize(glm::vec2 size)
{
    this->size = size;
    constructSurface();
}

void UIComponent::setPositionAndSize(glm::vec2 position, glm::vec2 size)
{
    setPosition(position);
    setSize(size);
}

void UIComponent::constructSurface()
{
    if (!surface) surface = new Surface();

    surface->getVertices() = 
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(glm::vec2(0.0f, size.y), 0.0f),
        glm::vec3(glm::vec2(size.x, 0.0f), 0.0f),
        glm::vec3(size, 0.0f)
    };

    surface->getTextureCoordinates() =
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    surface->getIndices() = {0, 1, 2, 3};

    surface->calculateSizesAndLength();
}

void UIComponent::clearComponents()
{
    for (unsigned int i = 0; i<components.size(); i++)
    {
        delete components[i];
    }
    components.clear();
}

void UIComponent::preRender()
{
}

GLenum UIComponent::getRenderMode()
{
    return GL_TRIANGLE_STRIP;
}

bool UIComponent::shouldCheckMouseClickEventForChildren()
{
    return true;
}

glm::vec2 UIComponent::getPosition()
{
    return position;
}

glm::vec2 UIComponent::getSize()
{
    return size;
}