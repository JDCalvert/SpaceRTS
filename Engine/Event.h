#ifndef MOUSE_EVENT
#define MOUSE_EVENT

#include <glm\glm.hpp>

enum EventType {MOUSE, KEY, TEXT};

struct Event
{
    int type;

    Event(EventType type)
    {
        this->type = type;
    }
};

struct MouseClickEvent : Event
{
    glm::vec2 position;
    int button;
    int action;

    MouseClickEvent(glm::vec2 position, int button, int action) : position(position), button(button), action(action), Event(MOUSE)
    {
    }

    MouseClickEvent* getRelative(glm::vec2 relative)
    {
        return new MouseClickEvent {position - relative, button, action};
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