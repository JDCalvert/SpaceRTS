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
    char charWidths[256];
    char base;
    GLuint textureId;

    static std::unordered_map<std::string, Font> fonts;

    static void loadFont(const char* bffFilePath, std::string fontName);
    static Font& getFont(std::string fontName);
};

#endif FONT_H