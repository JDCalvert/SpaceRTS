#include "UILabel.h"

void UILabel::processMouseEvent(MouseEvent* mouseEvent)
{
    //Do nothing
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

void UILabel::setText(std::string text, float height, Font font, Alignment alignment)
{
    setText(text);
    setHeight(height);
    setFont(font);
    setAlignment(alignment);

    recalculateSurface();
}

void UILabel::recalculateSurface()
{
    if (surface) delete surface;
    surface = new Surface();

    //First, work out how wide the string will be and find where it should start, based on the alignment
    float textWidth = 0.0f;
    for (unsigned int i = 0; i<text.length(); i++)
    {
        char character = text[i];
        textWidth += (float)font.charWidths[character] / font.cellWidth * height;
    }

    float xpos = 0.0f;
    if (alignment == CENTRE)
    {
        xpos = (size.x - textWidth) / 2;
    }
    if (alignment == RIGHT)
    {
        xpos = size.x - textWidth;
    }

    std::vector<glm::vec3>& vertices = surface->getVertices();
    std::vector<glm::vec2>& textureCoordinates = surface->getTextureCoordinates();
    std::vector<unsigned int>& indices = surface->getIndices();

    //Build up panels to display each letter
    for (unsigned int i = 0; i<text.length(); i++)
    {
        char character = text[i];
        float characterWidth = (float)font.charWidths[character] / font.cellWidth;

        float textureCoordinatesX = (character % font.rowWidth) * font.cellWidthNormalised;
        float textureCoordinatesY = (character / font.rowWidth) * font.cellHeightNormalised;

        float textureWidth = characterWidth / font.rowWidth;

        unsigned int baseIndex = vertices.size();

        vertices.push_back(glm::vec3(xpos, 0.0f, 0.0f));
        vertices.push_back(glm::vec3(xpos, height, 0.0f));
        vertices.push_back(glm::vec3(xpos + characterWidth * height, height, 0.0f));
        vertices.push_back(glm::vec3(xpos + characterWidth * height, 0.0f, 0.0f));

        textureCoordinates.push_back(glm::vec2(textureCoordinatesX, textureCoordinatesY));
        textureCoordinates.push_back(glm::vec2(textureCoordinatesX, textureCoordinatesY + font.cellHeightNormalised));
        textureCoordinates.push_back(glm::vec2(textureCoordinatesX + textureWidth, textureCoordinatesY + font.cellHeightNormalised));
        textureCoordinates.push_back(glm::vec2(textureCoordinatesX + textureWidth, textureCoordinatesY));

        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);

        xpos += characterWidth * height;
    }

    surface->diffuseMap = font.textureId;
    surface->calculateSizesAndLength();
}

GLenum UILabel::getRenderMode()
{
    return GL_TRIANGLES;
}