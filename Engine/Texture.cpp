#include "Texture.h"

#include <stdio.h>
#include <cstring>
#include <cstdlib>

#define HEADER_SIZE_DDS 124
#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

std::unordered_map<std::string, GLuint> Texture::textures;

GLuint Texture::getTexture(std::string textureName)
{
    return textures[textureName];
}

/**
* Load in a DDS file and output the texture identifier
*/
void Texture::loadDDS(const char* imagePath, std::string textureName)
{
    unsigned char header[HEADER_SIZE_DDS];

    FILE* file;
    fopen_s(&file, imagePath, "rb");
    if (!file)
    {
        printf("Failed to open DDS file");
        return;
    }

    char fileCode[4];
    fread(fileCode, 1, 4, file);
    if (strncmp(fileCode, "DDS ", 4) != 0)
    {
        printf("Failed to verify DDS header");
        fclose(file);
        return;
    }

    fread(header, HEADER_SIZE_DDS, 1, file);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char* buffer;
    unsigned int bufferSize;

    //Some varients output 0 as the number of mipmaps when there is only the main texture, we need to have at least 1
    mipMapCount = mipMapCount == 0 ? 1 : mipMapCount;

    //If we have at least one mipmap, then the total length of the image data will be less than double the length of the original image
    if (mipMapCount > 1)
    {
        bufferSize = linearSize * 2;
    }
    else
    {
        bufferSize = linearSize;
    }

    buffer = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
    fread(buffer, 1, bufferSize, file);
    fclose(file);

    unsigned int components = 4;
    if (fourCC == FOURCC_DXT1)
    {
        components = 3;
    }

    unsigned int format;
    switch (fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        return;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    if (mipMapCount == 1)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    }

    unsigned int blockSize = 16;
    if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
    {
        blockSize = 8;
    }

    unsigned int offset = 0;
    for (unsigned int level = 0; level<mipMapCount && (width || height); level++)
    {
        unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

        //Move onto the next mipmap level that starts immediately after, and is half the size in each dimension.
        offset += size;
        width /= 2;
        height /= 2;
    }
    free(buffer);

    textures[textureName] = textureId;
}