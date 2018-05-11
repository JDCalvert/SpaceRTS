#include "Font.h"

#define BFG_RS_NONE  0x0
#define BFG_RS_ALPHA 0x1
#define BFG_RS_RGB   0x2
#define BFG_RS_RGBA  0x4

#define ID_OFFSET	0
#define IMAGE_WIDTH_OFFSET	  2
#define IMAGE_HEIGHT_OFFSET	  6
#define	CELL_WIDTH_OFFSET	 10
#define CELL_HEIGHT_OFFSET	 14
#define BPP_OFFSET			 18
#define BASE_CHAR_OFFSET	 19
#define WIDTH_DATA_OFFSET	 20
#define MAP_DATA_OFFSET		276

std::unordered_map<std::string, Font> Font::fonts;

Font& Font::getFont(std::string fontName)
{
    return fonts[fontName];
}

void Font::loadFont(const char* bffFilePath, std::string fontName)
{
    char* data;
    char* image;
    std::fstream in;
    char bpp;
    int imageWidth, imageHeight;

    in.open(bffFilePath, std::ios_base::binary | std::ios_base::in);

    //Go to the end of the file. Our position is the file size, so cache that, then go back to the beginning.
    in.seekg(0, std::ios_base::end);
    unsigned long fileSize = (unsigned long)in.tellg();
    in.seekg(0, std::ios_base::beg);

    data = new char[fileSize];

    in.read(data, fileSize);
    in.close();

    unsigned char firstChar = data[0];
    unsigned char secondChar = data[1];
    if (firstChar != 0xBF || secondChar != 0xF2)
    {
        printf("The file is not the correct format. Not continuing");
        delete[] data;
        return;
    }

    int cellWidth, cellHeight;
    char base;

    unsigned int size = sizeof(int);
    memcpy(&imageWidth, &data[IMAGE_WIDTH_OFFSET], size);
    memcpy(&imageHeight, &data[IMAGE_HEIGHT_OFFSET], size);
    memcpy(&cellWidth, &data[CELL_WIDTH_OFFSET], size);
    memcpy(&cellHeight, &data[CELL_HEIGHT_OFFSET], size);
    bpp = data[BPP_OFFSET];
    base = data[BASE_CHAR_OFFSET];

    int rowWidth = imageWidth / cellWidth;
    float cellWidthNormalised = (float)cellWidth / (float)imageWidth;
    float cellHeightNormalised = (float)cellHeight / (float)imageHeight;

    int renderStyle;

    switch (bpp)
    {
    case 8:
        renderStyle = BFG_RS_ALPHA;
        break;
    case 24:
        renderStyle = BFG_RS_RGB;
        break;
    case 32:
        renderStyle = BFG_RS_RGBA;
        break;
    default:
        printf("The file isn't of a format we recognise. Not continuing");
        return;
    }

    unsigned int imageSize = imageWidth * imageHeight * (bpp / 8);
    image = new char[imageSize];

    unsigned char charWidths[256];
    memcpy(charWidths, &data[WIDTH_DATA_OFFSET], 256);
    memcpy(image, &data[MAP_DATA_OFFSET], imageSize);

    GLuint textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    switch (renderStyle)
    {
    case BFG_RS_ALPHA:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imageWidth, imageHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
        break;
    case BFG_RS_RGB:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        break;
    case BFG_RS_RGBA:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    delete image;
    delete data;

    Font& font = fonts[fontName];
    font.cellWidth = cellWidth;
    font.cellHeight = cellHeight;
    font.rowWidth = rowWidth;
    memcpy(font.charWidths, charWidths, sizeof(charWidths));
    font.base = base;
    font.textureId = textureId;
    font.cellWidthNormalised = cellWidthNormalised;
    font.cellHeightNormalised = cellHeightNormalised;
}