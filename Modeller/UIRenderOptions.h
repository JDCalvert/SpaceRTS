#ifndef UI_RENDER_OPTIONS_H
#define UI_RENDER_OPTIONS_H

#include "UIPanel.h"

class UIRenderOptions : public UIPanel
{
public:
    bool renderSurface = true;
    bool renderLines = true;
    bool renderPoints = true;

    float buttonSize;

    void build();
   
private:
    void addToggleButton(bool& toggle, glm::vec2 position, GLuint texture);
};

#endif
