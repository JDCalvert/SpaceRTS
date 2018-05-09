#ifndef UI_NUMBER_H
#define UI_NUMBER_H

#include "UITextBox.h"

class UINumber : public UITextBox
{
public:
    UINumber(float& value);

    void preRender();

private:
    float& value;
    float previousValue;
};

#endif
