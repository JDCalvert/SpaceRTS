#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "MouseEvent.h"

class UIComponent
{
public:
    glm::vec2 position;
    glm::vec2 size;

    virtual bool checkAndProcessMouseEvent(MouseEvent* mouseEvent) final;

protected:
    virtual bool processMouseEvent(MouseEvent* mouseEvent) = 0;

private:
    bool isClicked(MouseEvent* mouseEvent);
};

#endif
