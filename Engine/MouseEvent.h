#ifndef MOUSE_EVENT
#define MOUSE_EVENT

#include <glm\glm.hpp>

struct MouseEvent
{
    glm::vec2 position;
    int button;
    int action;

    MouseEvent* getRelative(glm::vec2 relative)
    {
        return new MouseEvent {position - relative, button, action};
    }
};

#endif