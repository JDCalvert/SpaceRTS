#ifndef UI_INFORMATION_H
#define UI_INFORMATION_H

#include <Font.h>
#include <UIPanel.h>
#include <UIButton.h>
#include <UIActionListener.h>

class UIInformation : public UIPanel, UIActionListener
{
public:
    Font* font;
    float textSize;
    float border;

    bool shouldRebuild;

    float newButtonSize;
    float newButtonXpos;

    GLuint texture;

    UIInformation(Surface* infoSurface);

    virtual void build() = 0;

    EventStatus processMouseScroll(MouseScrollEvent mouseEvent) override;

protected:
    Surface* infoSurface;

    int currentItem;
    int numItemsDisplay;
    int numItemsTotal;

    float ypos;

    UIButton* newButton;

    void addPanels();
    
    virtual int getNumItemsTotal() = 0;
    virtual UIComponent* addPanel(unsigned int i) = 0;

private:
    void addNewButton();
};

#endif