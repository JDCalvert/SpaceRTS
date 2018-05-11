#ifndef UI_NUMBER_H
#define UI_NUMBER_H

#include "UITextBox.h"

class UINumber : public UITextBox
{
public:
    UINumber(float& value);

    void preRender();

    void processTextEvent(TextEvent* event);

private:
    const char acceptableChars[12] {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '-'};

    float& value;
    float previousValue;

    bool isAcceptableChar(char newChar);
    void processNotActive();

    void recalculateSurface();
};

#endif
