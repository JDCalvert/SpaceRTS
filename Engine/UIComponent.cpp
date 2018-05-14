#include "UIComponent.h"

#include "Geometry.h"
#include "UserInterface.h"

UIComponent::~UIComponent()
{
    clearComponents();
}

UIComponent* UIComponent::checkAndProcessMouseEvent(MouseClickEvent* mouseEvent)
{
    //If this wasn't clicked, then none of the subcomponents will have been.
    if (!isClicked(mouseEvent)) return nullptr;

    if (shouldCheckMouseEventForChildren())
    {
        MouseClickEvent* relativeMouseEvent = mouseEvent->getRelative(position);

        //Go through our sub-components and, check if any were clicked. If so process and then return that component.
        for (auto i = components.begin(); i != components.end(); i++)
        {
            UIComponent* component = *i;
            UIComponent* eventComponent = component->checkAndProcessMouseEvent(relativeMouseEvent);
        
            if (eventComponent) return eventComponent;
        }
    }

    //If none of our child components were clicked, then this was clicked
    processMouseEvent(mouseEvent);
    return this;
}

void UIComponent::checkHover(glm::vec2 mousePosition)
{
    if (Geometry::isPointInBox(mousePosition, position, size))
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

bool UIComponent::isClicked(MouseClickEvent* mouseEvent)
{
    return Geometry::isPointInBox(mouseEvent->position, position, size);
}

void UIComponent::processMouseEvent(MouseClickEvent* mouseEvent)
{
}

void UIComponent::processKeyEvent(KeyEvent* keyEvent)
{
}

void UIComponent::processTextEvent(TextEvent* textEvent)
{
}

void UIComponent::becomeInactive()
{
    UserInterface* ui = UserInterface::getInstance();
    ui->clearActiveComponent(this);
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

void UIComponent::addComponent(UIComponent* component)
{
    components.push_back(component);
}

void UIComponent::setPosition(glm::vec2 position)
{
    this->position = position;
}

void UIComponent::setSize(glm::vec2 size)
{
    this->size = size;
}

void UIComponent::setPositionAndSize(glm::vec2 position, glm::vec2 size)
{
    setPosition(position);
    setSize(size);
    constructSurface();
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

bool UIComponent::shouldCheckMouseEventForChildren()
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