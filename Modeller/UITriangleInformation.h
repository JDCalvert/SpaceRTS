#ifndef UI_TRIANGLE_INFORMATION_H
#define UI_TRIANGLE_INFORMATION_H

#include <Font.h>

#include "UIInformation.h"
#include "UITrianglePanel.h"

class UITriangleInformation : public UIInformation
{
public:
    float indexWidth;

    UITriangleInformation(Surface* surface);
    
    void build() override;
    void preRender() override;

    std::vector<UITrianglePanel*>& getTrianglePanels();
    int getNumItemsTotal() override;

    void actionPerformed(UIComponent* component);

private:
    std::vector<UITrianglePanel*> trianglePanels;

    float xpos;

    void addHeader(std::string header);
    UIComponent* addPanel(unsigned int index);
};

#endif