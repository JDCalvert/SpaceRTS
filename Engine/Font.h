#ifndef FONT_H
#define FONT_H

#include <unordered_map>

#include <gl\glew.h>

#include <fstream>

struct Font
{
public:
    int cellWidth, cellHeight, rowWidth;
    float cellWidthNormalised, cellHeightNormalised;
    unsigned char charWidths[256];
    char base;
    GLuint textureId;

    static void loadFont(const char* bffFilePath, std::string fontName);
    static Font& getFont(std::string fontName);

private:
    static std::unordered_map<std::string, Font> fonts;
};

#endif FONT_H