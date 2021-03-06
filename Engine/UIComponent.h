#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <vector>

#include <gl\glew.h>

#include "Event.h"
#include "Surface.h"
#include "UILayout.h"
#include "UIActionListener.h"

class UIComponent
{
public:
    ~UIComponent();

    std::vector<UIComponent*> components;

    Surface surface;
    UILayout* layout;
    
    bool hover = false;
    void checkHover(glm::vec2 mousePosition);
    void setNoHover();

    void addComponent(UIComponent* component);
    void setActionListener(UIActionListener* actionListener);

    glm::vec2 getPosition();
    glm::vec2 getSize();
    
    virtual void setPosition(float x, float y);
    virtual void setPosition(glm::vec2 position);
    
    virtual void setSize(float x, float y);
    virtual void setSize(glm::vec2 size);

    virtual void setPositionAndSize(glm::vec2 position, glm::vec2 size) final;

    virtual UIComponent* checkAndProcessMouseClickEvent(MouseClickEvent mouseEvent) final;
    virtual EventStatus checkAndProcessMouseScrollEvent(MouseScrollEvent mouseEvent) final;

    virtual void processKeyEvent(KeyEvent keyEvent);
    virtual void processTextEvent(TextEvent textEvent);
    
    virtual void processNotActive();
    virtual bool shouldRemainActive();

    virtual void preRender();
    virtual GLenum getRenderMode();

protected:
    glm::vec2 position;
    glm::vec2 size;

    virtual void constructSurface();
    void clearComponents();
    void invokeActionListener();

    virtual void recalculateLayout() final;

    bool isChild(UIComponent* component);

    virtual void becomeInactive();
    virtual bool shouldCheckMouseClickEventForChildren();
    virtual void processMouseClick(MouseClickEvent mouseEvent);
    virtual EventStatus processMouseScroll(MouseScrollEvent mouseEvent);

private:
    UIActionListener* actionListener;

    bool isPointOnMe(glm::vec2 point);
};

#endif
