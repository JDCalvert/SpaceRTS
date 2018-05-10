#ifndef MOUSE_EVENT
#define MOUSE_EVENT

#include <glm\glm.hpp>

enum EventType {MOUSE, TEXT};

struct Event
{
    int type;

    Event(EventType type)
    {
        this->type = type;
    }
};

struct MouseEvent : Event
{
    glm::vec2 position;
    int button;
    int action;

    MouseEvent() : Event(MOUSE)
    {
    }

    MouseEvent* getRelative(glm::vec2 relative)
    {
        return new MouseEvent {position - relative, button, action};
    }
};

struct KeyEvent : Event
{
    unsigned int codepoint;
};

#endif