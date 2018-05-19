#include "UINumber.h"

#include <sstream>
#include <iomanip>

UINumber::UINumber(float& value) : value(value), UITextBox()
{
    previousValue = value;
    numDigits = 3;
}

void UINumber::preRender()
{
    if (previousValue != value)
    {
        recalculateSurface();
    }

    previousValue = value;
}

void UINumber::recalculateSurface()
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(numDigits) << value;
    std::string str = ss.str();

    label->setText(str);
    label->recalculateSurface();
}

void UINumber::processTextEvent(TextEvent event)
{
    if (!isAcceptableChar(event.codepoint)) return;

    UITextBox::processTextEvent(event);
}

bool UINumber::isAcceptableChar(char newChar)
{
    const char* begin = std::begin(acceptableChars);
    const char* end = std::end(acceptableChars);
    return std::find(begin, end, newChar) != end;
}

void UINumber::processNotActive()
{
    std::string str = label->getText();

    try
    {
        value = std::stof(str);
    }
    catch (std::invalid_argument e) {}
    catch (std::out_of_range e) {}

    recalculateSurface();
}

void UINumber::setText(float size, Font font, Alignment alignment)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(numDigits) << value;
    std::string str = ss.str();

    UITextBox::setText(str, size, font, alignment);
}

void UINumber::setNumDigits(int numDigits)
{
    this->numDigits = numDigits;
}