#ifndef UI_INFORMATION_H
#define UI_INFORMATION_H

#include <Font.h>
#include <UIPanel.h>

class UIInformation : public UIPanel
{
public:
    Font* font;
    float textSize;
    float border;

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

    void addPanels();
    
    virtual int getNumItemsTotal() = 0;
    virtual UIComponent* addPanel(unsigned int i) = 0;
};

#endif