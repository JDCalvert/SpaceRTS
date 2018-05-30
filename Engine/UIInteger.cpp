#include "UIInteger.h"

#include <sstream>
#include <iomanip>

UIInteger::UIInteger(int& value): value(value), UITextBox()
{
    previousValue = value;
}

void UIInteger::preRender()
{
    if (previousValue != value)
    {
        recalculateSurface();
    }

    previousValue = value;
}

void UIInteger::recalculateSurface()
{
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();

    label->setText(str);
    label->constructSurface();
}

void UIInteger::processTextEvent(TextEvent event)
{
    if (!isAcceptableChar(event.codepoint)) return;

    UITextBox::processTextEvent(event);
}

bool UIInteger::isAcceptableChar(char newChar)
{
    const char* begin = std::begin(acceptableChars);
    const char* end = std::end(acceptableChars);
    return std::find(begin, end, newChar) != end;
}

void UIInteger::processNotActive()
{
    std::string str = label->getText();

    try
    {
        value = std::stoi(str);
    }
    catch (std::invalid_argument e) {
    }
    catch (std::out_of_range e) {
    }
}

void UIInteger::setText(float size, Font font, Alignment alignment)
{
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();

    UITextBox::setText(str, size, font, alignment);
}