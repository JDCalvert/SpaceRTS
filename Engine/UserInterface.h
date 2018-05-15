#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <vector>

class UIComponent;

class UserInterface
{
public:
    void addComponent(UIComponent* component);
    
    virtual void render();
    void handleEvents();
    
    bool hasActiveComponent();
    UIComponent* getActiveComponent();
    void clearActiveComponent();
    void clearActiveComponent(UIComponent* component);

    static UserInterface* getInstance();

    template <class T>
    static T* initialise(T* ui)
    {
        instance = ui;
        return ui;
    }

protected:
    static UserInterface* instance;

private:

    UIComponent* activeComponent;
    std::vector<UIComponent*> components;
};

#endif