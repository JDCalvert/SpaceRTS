#ifndef UI_INTEGER_H
#define UI_INTEGER_H

#include "UITextBox.h"

class UIInteger: public UITextBox
{
public:
    UIInteger(int& value);

    void preRender() override;
    void setText(float size, Font font, Alignment alignment);

    void processTextEvent(TextEvent event) override;

private:
    const char acceptableChars[11]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-'};

    int& value;
    int previousValue;

    bool isAcceptableChar(char newChar);
    void recalculateSurface();

    void processNotActive() override;
};

#endif
