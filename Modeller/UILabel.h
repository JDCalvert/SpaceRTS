#ifndef UI_LABEL_H
#define UI_LABEL_H

#include <ResourceLoader.h>

#include "UIComponent.h"

enum Alignment {LEFT, CENTRE, RIGHT};

class UILabel : public UIComponent
{
public:
    void setText(std::string text, float size, Font font, Alignment alignment);
    
    GLenum getRenderMode();

protected:
    void processMouseEvent(MouseEvent* mouseEvent);

private:
    std::string text;

    Font font;
};

#endif
