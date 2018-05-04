#include "UILabel.h"

void UILabel::processMouseEvent(MouseEvent* mouseEvent)
{
    //Do nothing
}

void UILabel::setText(std::string text, float size, Font font)
{
    this->text = text;

    if (surface) delete surface;
    surface = new Surface();

    std::vector<glm::vec3>& vertices = surface->getVertices();
    std::vector<glm::vec2>& textureCoordinates = surface->getTextureCoordinates();
    std::vector<unsigned int>& indices = surface->getIndices();

    //Build up panels to display each letter
    float xpos = 0;
    for (unsigned int i = 0; i<text.length(); i++)
    {
        char character = text[i];
        float characterWidth = (float)font.charWidths[character] / font.cellWidth;

        float textureCoordinatesX = (character % font.rowWidth) * font.cellWidthNormalised;
        float textureCoordinatesY = (character / font.rowWidth) * font.cellHeightNormalised;

        float textureWidth = characterWidth / font.rowWidth;

        unsigned int baseIndex = vertices.size();

        vertices.push_back(glm::vec3(xpos, 0.0f, 0.0f));
        vertices.push_back(glm::vec3(xpos, size, 0.0f));
        vertices.push_back(glm::vec3(xpos + characterWidth * size, size, 0.0f));
        vertices.push_back(glm::vec3(xpos + characterWidth * size, 0.0f, 0.0f));

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

        xpos += characterWidth * size;
    }

    surface->calculateSizesAndLength();
}

GLenum UILabel::getRenderMode()
{
    return GL_TRIANGLES;
}