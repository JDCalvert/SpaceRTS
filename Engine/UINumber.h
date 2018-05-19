#ifndef UI_NUMBER_H
#define UI_NUMBER_H

#include "UITextBox.h"

class UINumber : public UITextBox
{
public:
    UINumber(float& value);

    void preRender() override;
    void setText(float size, Font font, Alignment alignment);
    void setNumDigits(int numDigits);

    void processTextEvent(TextEvent event) override;

private:
    const char acceptableChars[12] {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '-'};

    float& value;
    float previousValue;

    int numDigits;

    bool isAcceptableChar(char newChar);
    void recalculateSurface();
    
    void processNotActive() override;    
};

#endif
