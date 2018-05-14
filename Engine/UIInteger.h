#ifndef UI_INTEGER_H
#define UI_INTEGER_H

#include "UITextBox.h"

class UIInteger: public UITextBox
{
public:
    UIInteger(unsigned int& value);

    void preRender();

    void processTextEvent(TextEvent* event);

private:
    const char acceptableChars[12]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    unsigned int& value;
    unsigned int previousValue;

    bool isAcceptableChar(char newChar);
    void processNotActive();

    void recalculateSurface();
};

#endif
