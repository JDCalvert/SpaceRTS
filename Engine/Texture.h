#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <unordered_map>

#include <gl\glew.h>

struct Texture
{
    static std::unordered_map<std::string, GLuint> textures;

    static GLuint getTexture(std::string textureName);
    static void loadDDS(const char* imagePath, std::string textureName);
};

#endif