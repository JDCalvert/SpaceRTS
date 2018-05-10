#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <vector>

class UIComponent;

class UserInterface
{
public:
    UIComponent* activeComponent;

    std::vector<UIComponent*> components;

    void addComponent(UIComponent* component);
    void render();

    void handleEvents();
};

#endif