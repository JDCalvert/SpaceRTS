#ifndef UI_TRIANGLE_INFORMATION_H
#define UI_TRIANGLE_INFORMATION_H

#include <Font.h>
#include <UIPanel.h>

#include "UITrianglePanel.h"

class UITriangleInformation : public UIPanel
{
public:
    Font* font;
    float textSize;
    float border;
    float indexWidth;

    GLuint blankTexture;

    int startTriangle;
    int maxTriangles;

    UITriangleInformation(Surface* surface);
    
    void build();

    bool processMouseScroll(MouseScrollEvent* mouseEvent) override;

    std::vector<UITrianglePanel*>& getTrianglePanels();

private:
    Surface* infoSurface;
    std::vector<UITrianglePanel*> trianglePanels;

    float xpos, ypos;

    void addHeader(std::string header);
    void addTrianglePanel(Surface* infoSurface, unsigned int firstIndex);
};

#endif