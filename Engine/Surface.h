#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Bone
{
    glm::mat4 relative;
    glm::mat4 absolute;
    glm::mat4 inverseBind;
    int parent = -1;

    Bone(glm::mat4 relative, int parent)
    {
        this->relative = relative;
        this->parent = parent;
    }
};

class Surface
{
public:
    std::vector<Bone> bones;

    int verticesSize = 0;
    int textureCoordinatesSize = 0;
    int normalsSize = 0;
    int tangentsSize = 0;
    int bitangentsSize = 0;
    int boneIndicesSize = 0;
    int indicesSize = 0;

    int boneMatricesSize = 0;

    glm::vec3* verticesPointer = nullptr;
    glm::vec2* textureCoordinatesPointer = nullptr;
    glm::vec3* normalsPointer = nullptr;
    glm::vec3* tangentsPointer = nullptr;
    glm::vec3* bitangentsPointer = nullptr;
    glm::vec4* boneIndicesPointer = nullptr;
    unsigned int* indicesPointer = nullptr;

    glm::mat4* boneMatricesPointer = nullptr;

    GLfloat* ambientPointer = nullptr;
    GLfloat* diffusePointer = nullptr;
    GLfloat* specularPointer = nullptr;

    int length = 0;

    GLuint diffuseMap;
    GLuint normalMap;
    GLuint specularMap;

    Surface();

    //Load from file
    void loadFromFile(const char* objFilePath);
    void loadFromFile(const char* objFilePath, float scale);

    //Write to file
    void writeToFile(const char* objFilePath);
    void calculateTangents();
    
    //Set specific things about the texture
    void setTextures(GLuint diffuseMap, GLuint normalmap, GLuint specularMap);
    void setAmbientColour(glm::vec4 ambientColour);
    void setDiffuseColour(glm::vec4 diffuseColour);

    //Calculate things before the texture can be used
    void setUpColourPointers();
    void calculateSizesAndLength();
    void recalculateModelBoneMatrices();

    //The actual parts of the surface
    std::vector<glm::vec3>& getVertices();
    std::vector<glm::vec2>& getTextureCoordinates();
    std::vector<glm::vec3>& getNormals();
    std::vector<glm::vec3>& getTangents();
    std::vector<glm::vec3>& getBitangents();
    std::vector<unsigned int>& getIndices();
    std::vector<glm::vec4>& getBoneIndicesAndWeights();
    std::vector<Bone>& getBones();

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<glm::vec4> boneIndicesAndWeights;
    std::vector<unsigned int> indices;

    std::vector<glm::mat4> boneMatrices;

    glm::vec4 ambientColour;
    glm::vec4 diffuseColour;

    void loadObj(const char* path, std::vector<glm::mat4> &bones, std::vector<int> &parents);
    void prepareBones();
};

#endif