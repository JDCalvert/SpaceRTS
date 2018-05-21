#include "UserInterfaceModeller.h"
#include "UIVertexPanel.h"

UserInterfaceModeller::UserInterfaceModeller(Surface* infoSurface)
{
    this->infoSurface = infoSurface;
}

UserInterfaceModeller* UserInterfaceModeller::getInstance()
{
    return (UserInterfaceModeller*)UserInterface::getInstance();
}

void UserInterfaceModeller::build()
{
    renderOptions = new UIRenderOptions();
    renderOptions->build();
    addComponent(renderOptions);

    vertexInformation = new UIVertexInformation(infoSurface);
    vertexInformation->build();
    addComponent(vertexInformation);

    triangleInformation = new UITriangleInformation(infoSurface);
    triangleInformation->build();
    addComponent(triangleInformation);

    boneInformation = new UIBoneInformation(infoSurface);
    boneInformation->build();
    addComponent(boneInformation);

    saveLoadPanel = new UISaveLoadPanel();
    saveLoadPanel->build();
    addComponent(saveLoadPanel);
}

void UserInterfaceModeller::loadSurface(const char* fileName)
{
    infoSurface->loadFromFile(fileName);

    vertexInformation->build();
    triangleInformation->build();
    boneInformation->build();
}

void UserInterfaceModeller::saveSurface(const char* fileName)
{
    infoSurface->writeToFile(fileName);
}

std::vector<glm::vec3> UserInterfaceModeller::getHighlightVertices()
{
    std::vector<glm::vec3> highlights;
    std::vector<UIVertexPanel*>& vertexPanels = vertexInformation->getVertexPanels();
    for (auto i = vertexPanels.begin(); i != vertexPanels.end(); i++)
    {
        UIVertexPanel* vertexPanel = *i;
        if (vertexPanel->isHighlighted()) highlights.push_back(vertexPanel->vertexPosition);
    }

    return highlights;
}

std::vector<unsigned int> UserInterfaceModeller::getHighlightIndices()
{
    std::vector<unsigned int> highlights;
    std::vector<UITrianglePanel*>& trianglePanels = triangleInformation->getTrianglePanels();
    for (auto i = trianglePanels.begin(); i != trianglePanels.end(); i++)
    {
        UITrianglePanel* trianglePanel = *i;
        if (trianglePanel->isHighlighted()) trianglePanel->addIndices(highlights);
    }

    return highlights;
}

void UserInterfaceModeller::recalculateVertexPositions()
{
    //Update the absolute positions of all the bones
    std::vector<Bone>& bones = infoSurface->getBones();
    std::vector<glm::mat4> transformMatrices;
    std::vector<glm::mat3> transposeMatrices;
    for (int i=0; i<bones.size(); i++)
    {
        Bone& bone = bones[i];
        int parent = bone.parent;
        if (parent > -1)
        {
            bone.absolute = bones[parent].absolute * bone.relative;
        }
        else
        {
            bone.absolute = bone.relative;
        }

        glm::mat4 transformMatrix = bone.absolute * bone.inverseBind;
        glm::mat3 transposeMatrix = glm::transpose(glm::mat3(transformMatrix));

        bone.inverseBind = glm::inverse(bone.absolute);

        transformMatrices.push_back(transformMatrix);
        transposeMatrices.push_back(transposeMatrix);
    }

    if (boneInformation->updateVertices)
    {
        //Update all the vertices depending on those
        std::vector<glm::vec3>& vertices = infoSurface->getVertices();
        std::vector<glm::vec3>& normals = infoSurface->getNormals();
        std::vector<glm::vec3>& tangents = infoSurface->getTangents();
        std::vector<glm::vec3>& bitangents = infoSurface->getBitangents();
        std::vector<glm::vec4>& boneIndices = infoSurface->getBoneIndicesAndWeights();
        for (int i=0; i<vertices.size(); i++)
        {
            glm::vec3& vertex = vertices[i];
            glm::vec3& normal = normals[i];
            glm::vec3& tangent = tangents[i];
            glm::vec3& bitangent = bitangents[i];
        
            glm::vec3 newVertex;
            glm::vec3 newNormal;
            glm::vec3 newTangent;
            glm::vec3 newBitangent;

            for (unsigned int j=0; j<2; j++)
            {
                int parentIndex = boneIndices[i][j*2];
                float weight = boneIndices[i][j*2+1];
            
                if (parentIndex > -1)
                {
                    glm::mat4 transformMatrix = transformMatrices[parentIndex];
                    glm::mat3 transposeMatrix = transposeMatrices[parentIndex];

                    newVertex += glm::vec3((transformMatrix * glm::vec4(vertex, 1.0f)) * weight);
                    newNormal += (transposeMatrix * normal) * weight;
                    newTangent += (transposeMatrix * tangent) * weight;
                    newBitangent += (transposeMatrix * bitangent) * weight;
                }
            }

            vertex = newVertex;
            normal = newNormal;
            tangent = newTangent;
            bitangent = newBitangent;
        }
    }
}

void UserInterfaceModeller::newVertex()
{
    unsigned int activeBone = boneInformation->activeBone;
    Bone& bone = infoSurface->getBones()[activeBone];
    glm::mat4 transform = bone.absolute;
    glm::mat3 transpose = glm::transpose(glm::mat3(transform));

    glm::vec3 vertexPosition(transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    glm::vec2 vertexTextureCoordinates(0.0f, 0.0f);
    glm::vec3 vertexNormal(transpose * glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec4 boneIndexAndWeight(activeBone, 1.0f, -1, 0.0f);

    infoSurface->getVertices().push_back(vertexPosition);
    infoSurface->getTextureCoordinates().push_back(vertexTextureCoordinates);
    infoSurface->getNormals().push_back(vertexNormal);
    infoSurface->getBoneIndicesAndWeights().push_back(boneIndexAndWeight);

    infoSurface->calculateTangents();
    infoSurface->calculateSizesAndLength();
}

void UserInterfaceModeller::newBone()
{
    unsigned int activeBone = boneInformation->activeBone;
    Bone newBone(glm::mat4(), activeBone);

    infoSurface->getBones().push_back(newBone);

    infoSurface->prepareBones();
}