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

        transformMatrices.push_back(transformMatrix);
        transposeMatrices.push_back(transposeMatrix);
    }

    //Update all the vertices depending on those
    std::vector<glm::vec3>& vertices = infoSurface->getVertices();
    std::vector<glm::vec3>& normals = infoSurface->getNormals();
    std::vector<glm::vec3>& tangents = infoSurface->getTangents();
    std::vector<glm::vec3>& bitangents = infoSurface->getBitangents();
    std::vector<glm::vec4>& boneIndices = infoSurface->getBoneIndicesAndWeights();
    for (int i=0; i<vertices.size(); i++)
    {
        glm::vec3& vertex = vertices[i];
        
        int parentIndex[2] {boneIndices[i][0], boneIndices[i][2]};

        glm::mat4 transformMatrix[2];
        glm::mat3 transposeMatrix[2];

        for (unsigned int j=0; j<2; j++)
        {
            if (parentIndex[j])
            {
                transformMatrix[j] = transformMatrices[parentIndex[j]];
                transposeMatrix[j] = transposeMatrices[parentIndex[j]];
            }
        }

        vertex = 
    }

}