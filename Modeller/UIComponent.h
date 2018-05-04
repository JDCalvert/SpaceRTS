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

    glm::vec2 getPosition();

    void addComponent(UIComponent* component);
    void setPositionAndSize(glm::vec2 position, glm::vec2 size);

    virtual bool checkAndProcessMouseEvent(MouseEvent* mouseEvent) final;
    virtual GLenum getRenderMode();

protected:
    virtual void processMouseEvent(MouseEvent* mouseEvent) = 0;
    
    void constructSurface();

private:
    glm::vec2 position;
    glm::vec2 size;

    bool isClicked(MouseEvent* mouseEvent);
};

#endif
