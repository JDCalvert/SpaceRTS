#ifndef RESOURCE_LOADER
#define RESOURCE_LOADER

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL\glew.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>

/**
* ResourceLoader
* Class for loading resources like textures and shaders
*/
class ResourceLoader
{
public:
    static const int HEADER_SIZE_DDS = 124;
    static const int FOURCC_DXT1 = 0x31545844;
    static const int FOURCC_DXT3 = 0x33545844;
    static const int FOURCC_DXT5 = 0x35545844;

    /**
    * Load in a DDS file and output the texture identifier
    */
    static GLuint loadDDS(const char* imagePath)
    {
        unsigned char header[HEADER_SIZE_DDS];

        FILE* file;
        fopen_s(&file, imagePath, "rb");
        if (!file)
        {
            printf("Failed to open DDS file");
            return 0;
        }

        char fileCode[4];
        fread(fileCode, 1, 4, file);
        if (strncmp(fileCode, "DDS ", 4) != 0)
        {
            printf("Failed to verify DDS header");
            fclose(file);
            return 0;
        }

        fread(header, HEADER_SIZE_DDS, 1, file);

        unsigned int height = *(unsigned int*)&(header[8]);
        unsigned int width = *(unsigned int*)&(header[12]);
        unsigned int linearSize = *(unsigned int*)&(header[16]);
        unsigned int mipMapCount = *(unsigned int*)&(header[24]);
        unsigned int fourCC = *(unsigned int*)&(header[80]);

        unsigned char* buffer;
        unsigned int bufferSize;

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
            return 0;
        }

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

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

        return textureId;
    }

    static std::string readFromFile(const char* path)
    {
        std::string output;

        std::ifstream stream(path, std::ios::in);
        if (!stream.is_open())
        {
            printf("Unable to read %s\n", path);
            getchar();
            return 0;
        }

        std::string line = "";
        while (std::getline(stream, line))
        {
            output += "\n" + line;
        }
        stream.close();

        return output;
    }
};

#endif