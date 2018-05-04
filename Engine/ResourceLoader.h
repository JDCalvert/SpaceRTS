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

struct Font
{
    int cellWidth, cellHeight, rowWidth;
    float cellWidthNormalised, cellHeightNormalised;
    char charWidths[256];
    char base;
    GLuint textureId;
};

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

    static void loadObj(const char* path, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textureCoordinates, std::vector<glm::vec3> &normals,
        std::vector<glm::vec3> &tangents, std::vector<glm::vec3> &bitangents, std::vector<glm::vec4> &boneIndices,
        std::vector<unsigned int> &indices, std::vector<glm::mat4> &bones, std::vector<int> &parents)
    {
        //Open the file as a binary, and start at the end so we can get the length, then go back to the beginning.
        std::ifstream in(path, std::ios::in | std::ios::binary | std::ios::ate);
        unsigned int fileSize = (unsigned int)in.tellg();
        in.seekg(0, std::ios::beg);

        //Read in the file then close
        char* data = new char[fileSize];
        in.read(data, fileSize);
        in.close();

        int lengthsSize = 3 * sizeof(unsigned int);
        unsigned int* lengths = new unsigned int[3];
        memcpy(lengths, data, lengthsSize);

        int verticesSize = lengths[0] * sizeof(glm::vec3);
        int textureCoordinatesSize = lengths[0] * sizeof(glm::vec2);
        int normalsSize = lengths[0] * sizeof(glm::vec3);
        int tangentsSize = lengths[0] * sizeof(glm::vec3);
        int bitangentsSize = lengths[0] * sizeof(glm::vec3);
        int boneIndicesSize = lengths[0] * sizeof(glm::vec4);
        int indicesSize = lengths[1] * sizeof(unsigned int);
        int bonesSize = lengths[2] * sizeof(glm::mat4);
        int parentsSize = lengths[2] * sizeof(int);

        vertices.resize(lengths[0]);
        textureCoordinates.resize(lengths[0]);
        normals.resize(lengths[0]);
        tangents.resize(lengths[0]);
        bitangents.resize(lengths[0]);
        boneIndices.resize(lengths[0]);
        indices.resize(lengths[1]);
        bones.resize(lengths[2]);
        parents.resize(lengths[2]);

        int verticesOffset = lengthsSize;
        int textureCoordinatesOffset = verticesOffset + verticesSize;
        int normalsOffset = textureCoordinatesOffset + textureCoordinatesSize;
        int tangentsOffset = normalsOffset + normalsSize;
        int bitangentsOffset = tangentsOffset + tangentsSize;
        int boneIndicesOffset = bitangentsOffset + bitangentsSize;
        int indicesOffset = boneIndicesOffset + boneIndicesSize;
        int bonesOffset = indicesOffset + indicesSize;
        int parentsOffset = bonesOffset + bonesSize;

        memcpy(&vertices[0], data + verticesOffset, verticesSize);
        memcpy(&textureCoordinates[0], data + textureCoordinatesOffset, textureCoordinatesSize);
        memcpy(&normals[0], data + normalsOffset, normalsSize);
        memcpy(&tangents[0], data + tangentsOffset, tangentsSize);
        memcpy(&bitangents[0], data + bitangentsOffset, bitangentsSize);
        memcpy(&boneIndices[0], data + boneIndicesOffset, boneIndicesSize);
        memcpy(&indices[0], data + indicesOffset, indicesSize);
        memcpy(&bones[0], data + bonesOffset, bonesSize);
        memcpy(&parents[0], data + parentsOffset, parentsSize);

        //Clean up
        delete lengths;
        delete data;
    }

    static Font* loadBFF(const char* texturePath)
    {
        char* data;
        char* image;
        std::fstream in;
        char bpp;
        int imageWidth, imageHeight;

        in.open(texturePath, std::ios_base::binary | std::ios_base::in);

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
            return 0;
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
            return 0;
        }

        unsigned int imageSize = imageWidth * imageHeight * (bpp / 8);
        image = new char[imageSize];

        char charWidths[256];
        memcpy(charWidths, &data[WIDTH_DATA_OFFSET], 256);
        memcpy(image, &data[MAP_DATA_OFFSET], imageSize);

        GLuint textureId;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

        Font* font = new Font();
        font->cellWidth = cellWidth;
        font->cellHeight = cellHeight;
        font->rowWidth = rowWidth;
        memcpy(font->charWidths, charWidths, sizeof(charWidths));
        font->base = base;
        font->textureId = textureId;
        font->cellWidthNormalised = cellWidthNormalised;
        font->cellHeightNormalised = cellHeightNormalised;

        return font;
    }
};

#endif