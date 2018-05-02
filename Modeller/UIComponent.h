#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <vector>

#include <MouseEvent.h>

class UIComponent
{
public:
    glm::vec2 position;
    glm::vec2 size;
    
    std::vector<UIComponent*> components;

    void addComponent(UIComponent* component);

    virtual bool checkAndProcessMouseEvent(MouseEvent* mouseEvent) final;

protected:
    virtual void processMouseEvent(MouseEvent* mouseEvent) = 0;

private:
    bool isClicked(MouseEvent* mouseEvent);
};

#endif
