#include "UIUnsignedInteger.h"

#include <sstream>
#include <iomanip>

UIUnsignedInteger::UIUnsignedInteger(unsigned int& value): value(value), UITextBox()
{
    previousValue = value;
}

void UIUnsignedInteger::preRender()
{
    if (previousValue != value)
    {
        recalculateSurface();
    }

    previousValue = value;
}

void UIUnsignedInteger::recalculateSurface()
{
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();

    label->setText(str);
    label->recalculateSurface();
}

void UIUnsignedInteger::processTextEvent(TextEvent event)
{
    if (!isAcceptableChar(event.codepoint)) return;

    UITextBox::processTextEvent(event);
}

bool UIUnsignedInteger::isAcceptableChar(char newChar)
{
    const char* begin = std::begin(acceptableChars);
    const char* end = std::end(acceptableChars);
    return std::find(begin, end, newChar) != end;
}

void UIUnsignedInteger::processNotActive()
{
    std::string str = label->getText();

    try
    {
        value = std::stof(str);
    }
    catch (std::invalid_argument e) {
    }
    catch (std::out_of_range e) {
    }
}