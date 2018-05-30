#include "UILabel.h"

UILabel* UILabel::addLabel(UIComponent* parent, float width, float height, std::string text, Font& font, Alignment alignment)
{
    UILabel* label = new UILabel();
    label->setSize(width, height);
    label->setText(text, height, font, alignment);
    parent->addComponent(label);

    return label;
}

void UILabel::setText(std::string text)
{
    this->text = text;
}

void UILabel::setHeight(float height)
{
    this->height = height;
}

void UILabel::setFont(Font font)
{
    this->font = font;
}

void UILabel::setAlignment(Alignment alignment)
{
    this->alignment = alignment;
}

void UILabel::setText(std::string text, float height, Font& font, Alignment alignment)
{
    setText(text);
    setHeight(height);
    setFont(font);
    setAlignment(alignment);

    constructSurface();
}

std::string UILabel::getText()
{
    return text;
}

void UILabel::addChar(char newChar)
{
    text += newChar;
    constructSurface();
}

void UILabel::removeLastChar()
{
    text = text.substr(0, text.length() - 1);
    constructSurface();
}

void UILabel::constructSurface()
{
    int numChars = text.length();

    //First, work out how wide the string will be and find where it should start, based on the alignment
    float textWidth = 0.0f;
    for (unsigned int i = 0; i<numChars; i++)
    {
        char character = text[i];
        textWidth += (float)font.charWidths[character] / font.cellWidth * height;
    }

    float xpos = 0.0f;
    if (alignment == CENTRE)
    {
        xpos = (size.x - textWidth) / 2;
    }
    else if (alignment == RIGHT)
    {
        xpos = size.x - textWidth;
    }

    std::vector<glm::vec3>& vertices = surface.getVertices();
    std::vector<glm::vec2>& textureCoordinates = surface.getTextureCoordinates();
    std::vector<unsigned int>& indices = surface.getIndices();

    vertices.resize(numChars * 4);
    textureCoordinates.resize(numChars * 4);
    indices.resize(numChars * 6);

    //Build up panels to display each letter
    for (unsigned int i = 0; i<numChars; i++)
    {
        char character = text[i];
        float characterWidth = (float)font.charWidths[character] / font.cellWidth;

        float textureCoordinatesX = (character % font.rowWidth) * font.cellWidthNormalised;
        float textureCoordinatesY = (character / font.rowWidth) * font.cellHeightNormalised;

        float textureWidth = characterWidth / font.rowWidth;

        vertices[i * 4 + 0] = glm::vec3(xpos, 0.0f, 0.0f);
        vertices[i * 4 + 1] = glm::vec3(xpos, height, 0.0f);
        vertices[i * 4 + 2] = glm::vec3(xpos + characterWidth * height, height, 0.0f);
        vertices[i * 4 + 3] = glm::vec3(xpos + characterWidth * height, 0.0f, 0.0f);

        textureCoordinates[i * 4 + 0] = glm::vec2(textureCoordinatesX, textureCoordinatesY);
        textureCoordinates[i * 4 + 1] = glm::vec2(textureCoordinatesX, textureCoordinatesY + font.cellHeightNormalised);
        textureCoordinates[i * 4 + 2] = glm::vec2(textureCoordinatesX + textureWidth, textureCoordinatesY + font.cellHeightNormalised);
        textureCoordinates[i * 4 + 3] = glm::vec2(textureCoordinatesX + textureWidth, textureCoordinatesY);

        indices[i * 6 + 0] = i * 4 + 0;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;
        indices[i * 6 + 3] = i * 4 + 0;
        indices[i * 6 + 4] = i * 4 + 2;
        indices[i * 6 + 5] = i * 4 + 3;

        xpos += characterWidth * height;
    }

    surface.diffuseMap = font.textureId;
    surface.calculateSizesAndLength();
}

GLenum UILabel::getRenderMode()
{
    return GL_TRIANGLES;
}