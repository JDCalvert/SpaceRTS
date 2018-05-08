#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <vector>

#include <gl\glew.h>

#include <MouseEvent.h>
#include <Surface.h>

class UIComponent
{
public:    
    std::vector<UIComponent*> components;

    Surface* surface;

    void addComponent(UIComponent* component);

    glm::vec2 getPosition();
    void setPosition(glm::vec2 position);
    
    glm::vec2 getSize();
    void setSize(glm::vec2 size);

    void setPositionAndSize(glm::vec2 position, glm::vec2 size);

    virtual bool checkAndProcessMouseEvent(MouseEvent* mouseEvent) final;
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
