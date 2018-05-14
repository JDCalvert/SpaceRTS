#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "Font.h"
#include "UIComponent.h"

enum Alignment {LEFT, CENTRE, RIGHT};

class UILabel : public UIComponent
{
public:
    void setText(std::string text, float textSize, Font& font, Alignment alignment);
    void setText(std::string text);
    void setHeight(float height);
    void setFont(Font font);
    void setAlignment(Alignment alignment);
    
    void recalculateSurface();
    GLenum getRenderMode();

    void addChar(char newChar);
    void removeLastChar();

    std::string getText();

private:
    std::string text;
    float height;
    Font font;
    Alignment alignment;
};

#endif