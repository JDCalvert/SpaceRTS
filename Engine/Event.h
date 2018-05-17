#ifndef MOUSE_EVENT
#define MOUSE_EVENT

#include <glm\glm.hpp>

enum EventType {MOUSE_CLICK, MOUSE_SCROLL, KEY, TEXT};
enum EventStatus {PROCESSED, HANDLED_NOT_PROCESSED, NOT_HANDLED};

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

    MouseEvent(glm::vec2 position, EventType type) : position(position), Event(type)
    {
    }
};

struct MouseClickEvent : MouseEvent
{
    int button;
    int action;

    MouseClickEvent(glm::vec2 position, int button, int action) : button(button), action(action), MouseEvent(position, MOUSE_CLICK)
    {
    }

    MouseClickEvent getRelative(glm::vec2 relative)
    {
        return MouseClickEvent {position - relative, button, action};
    }
};

struct MouseScrollEvent : MouseEvent
{
    double xOffset, yOffset;

    MouseScrollEvent(glm::vec2 position, double xOffset, double yOffset) : xOffset(xOffset), yOffset(yOffset), MouseEvent(position, MOUSE_SCROLL)
    {
    }

    MouseScrollEvent getRelative(glm::vec2 relative)
    {
        return MouseScrollEvent {position - relative, xOffset, yOffset};
    }
};

struct KeyEvent : Event
{
    int key;
    int scancode;
    int action;
    int mods;

    KeyEvent(int key, int scancode, int action, int mods) : key(key), scancode(scancode), action(action), mods(mods), Event(KEY)
    {
    }
};

struct TextEvent : Event
{
    unsigned int codepoint;

    TextEvent(int codepoint) : codepoint(codepoint), Event(TEXT)
    {
    }
};

#endif