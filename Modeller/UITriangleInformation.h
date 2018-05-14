#ifndef UI_TRIANGLE_INFORMATION_H
#define UI_TRIANGLE_INFORMATION_H

#include <Font.h>
#include <UIPanel.h>

class UITriangleInformation : UIPanel
{
public:
    Font* font;
    float textSize;
    float border;
    float indexWidth;

    void build(Surface* surface);

private:
    Surface* infoSurface;

    GLuint blankTexture;

    float xpos, ypos;

    void rebuildPanels();
    void addHeader(std::string header);
};

#endif