#ifndef UI_INTEGER_H
#define UI_INTEGER_H

#include "UITextBox.h"

class UIUnsignedInteger: public UITextBox
{
public:
    UIUnsignedInteger(unsigned int& value);

    void preRender() override;
    void setText(float size, Font font, Alignment alignment);

    void processTextEvent(TextEvent event) override;

private:
    const char acceptableChars[12]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    unsigned int& value;
    unsigned int previousValue;

    bool isAcceptableChar(char newChar);
    void recalculateSurface();

    void processNotActive() override;
};

#endif
