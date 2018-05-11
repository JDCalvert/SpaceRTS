#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <vector>

#include <gl\glew.h>

#include <Event.h>
#include <Surface.h>

class UIComponent
{
public:    
    std::vector<UIComponent*> components;

    Surface* surface;

    void addComponent(UIComponent* component);

    glm::vec2 getPosition();
    glm::vec2 getSize();
    
    virtual void setPosition(glm::vec2 position);
    virtual void setSize(glm::vec2 size);
    virtual void setPositionAndSize(glm::vec2 position, glm::vec2 size) final;

    virtual UIComponent* checkAndProcessMouseEvent(MouseEvent* mouseEvent) final;
    virtual void processKeyEvent(KeyEvent* keyEvent);
    virtual void processTextEvent(TextEvent* textEvent);
    
    virtual void processNotActive();
    virtual bool shouldRemainActive();

    virtual void preRender();
    virtual GLenum getRenderMode();

protected:
    glm::vec2 position;
    glm::vec2 size;

    virtual void becomeInactive();
    virtual bool shouldCheckMouseEventForChildren();
    virtual void processMouseEvent(MouseEvent* mouseEvent);
    
    void constructSurface();

private:
    bool isClicked(MouseEvent* mouseEvent);
};

#endif
