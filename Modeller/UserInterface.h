#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <vector>

class UIComponent;

class UserInterface
{
public:
    void addComponent(UIComponent* component);
    
    void render();
    void handleEvents();
    
    bool hasActiveComponent();
    void clearActiveComponent();
    void clearActiveComponent(UIComponent* component);

    static UserInterface* initialise();
    static UserInterface* getInstance();

private:
    static UserInterface* instance;

    std::vector<UIComponent*> components;

    UIComponent* activeComponent;
};

#endif