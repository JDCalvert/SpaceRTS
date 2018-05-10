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

    virtual bool checkAndProcessMouseEvent(MouseEvent* mouseEvent) final;

    virtual void preRender();
    virtual GLenum getRenderMode();

protected:
    glm::vec2 position;
    glm::vec2 size;

    virtual void processMouseEvent(MouseEvent* mouseEvent) = 0;
    
    void constructSurface();

private:
    bool isClicked(MouseEvent* mouseEvent);
};

#endif
