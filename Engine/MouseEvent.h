#ifndef MOUSE_EVENT
#define MOUSE_EVENT

#include <glm\glm.hpp>

struct MouseEvent
{
    glm::vec2 position;
    int button;
    int action;
};

#endif