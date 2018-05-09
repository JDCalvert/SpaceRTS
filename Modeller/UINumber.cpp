#include "UINumber.h"

#include <sstream>
#include <iomanip>

UINumber::UINumber(float& value) : value(value), UITextBox()
{
    previousValue = value;
}

void UINumber::preRender()
{
    if (previousValue != value)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << value;
        std::string str = ss.str();

        label->setText(str);
        label->recalculateSurface();
    }

    previousValue = value;
}

