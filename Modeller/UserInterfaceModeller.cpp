#include "UserInterfaceModeller.h"

#include <OpenGLContext.h>

#include "BlankShader.h"

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
    addComponent(vertexInformation);

    triangleInformation = new UITriangleInformation(infoSurface);
    addComponent(triangleInformation);

    boneInformation = new UIBoneInformation(infoSurface);
    addComponent(boneInformation);

    textureInformation = new UITextureInformation(infoSurface);
    textureInformation->build();
    addComponent(textureInformation);

    saveLoadPanel = new UISaveLoadPanel();
    saveLoadPanel->build();
    addComponent(saveLoadPanel);

    rebuildInformation();
}

void UserInterfaceModeller::rebuildInformation()
{
    vertexInformation->shouldRebuild = true;
    triangleInformation->shouldRebuild = true;
    boneInformation->shouldRebuild = true;
}

void UserInterfaceModeller::loadSurface(const char* fileName)
{
    infoSurface->loadFromFile(fileName);
    rebuildInformation();
}

void UserInterfaceModeller::saveSurface(const char* fileName)
{
    infoSurface->writeToFile(fileName);
}

void UserInterfaceModeller::importSurface(const char* fileName)
{
    //first, import the file as a new surface
    Surface* importSurface = new Surface();
    importSurface->loadFromFile(fileName);

    int activeBone = boneInformation->activeBone;

    std::vector<Bone>& infoBones = infoSurface->getBones();
    int numBones = infoBones.size();

    //Second, append the new surface's bones onto our surface
    //Ignore the first bone, as we'll use our active bone as the base
    std::vector<Bone>& importBones = importSurface->getBones();
    for (unsigned int i=1; i<importBones.size(); i++)
    {
        Bone newBone = importBones[i];
        int& parent = newBone.parent;

        if (parent == 0)
        {
            //This bone originally depended on the base bone, which is now the active bone
            parent = activeBone;
        }
        else
        {
            //The bone depended on another bone, which will be one that we added
            //Minus one because we haven't added the base bone
            parent += numBones - 1;
        }

        infoBones.push_back(newBone);
    }

    infoSurface->prepareBones();
    
    unsigned int newNumBones = infoBones.size();
    std::vector<glm::mat3> transposeMatrices(newNumBones);
    for (unsigned int i=0; i<newNumBones; i++)
    {
        transposeMatrices[i] = glm::transpose(glm::mat3(infoBones[i].absolute));
    }

    std::vector<glm::vec3>& infoVertices = infoSurface->getVertices();
    std::vector<glm::vec2>& infoTextureCoordinates = infoSurface->getTextureCoordinates();
    std::vector<glm::vec3>& infoNormals = infoSurface->getNormals();
    std::vector<glm::vec4>& infoBoneIndices = infoSurface->getBoneIndicesAndWeights();
    int numVertices = infoVertices.size();

    //Third, add all the vertices, transformed by the active bone matrix
    std::vector<glm::vec3>& importVertices = importSurface->getVertices();
    std::vector<glm::vec2>& importTextureCoordinates = importSurface->getTextureCoordinates();
    std::vector<glm::vec3>& importNormals = importSurface->getNormals();
    std::vector<glm::vec4>& importBoneIndicesAndWeights = importSurface->getBoneIndicesAndWeights();
    for (unsigned int i=0; i<importVertices.size(); i++)
    {
        glm::vec4 newVertex = infoBones[activeBone].absolute * glm::vec4(importVertices[i], 1.0f);
        glm::vec2 newTextureCoordinate = importTextureCoordinates[i];
        glm::vec3 newNormal = transposeMatrices[activeBone] * importNormals[i];
        glm::vec4 newBoneIndices = importBoneIndicesAndWeights[i];

        //As with the bones, if these depend on the base bone then use the active bone, otherwise
        //use the new bone that was added from the import surface.
        for (unsigned int j=0; j<4; j+=2)
        {
            if (newBoneIndices[j] == 0)
            {
                newBoneIndices[j] = activeBone;
            }
            else if (newBoneIndices[j] > 0)
            {
                newBoneIndices[j] += numBones - 1;
            }
        }

        infoVertices.push_back(newVertex);
        infoTextureCoordinates.push_back(newTextureCoordinate);
        infoNormals.push_back(newNormal);
        infoBoneIndices.push_back(newBoneIndices);
    }

    //Next, add all the triangles from the import surface to ours
    std::vector<unsigned int>& importIndices = importSurface->getIndices();
    std::vector<unsigned int>& infoIndices = infoSurface->getIndices();
    for (unsigned int i=0; i<importIndices.size(); i++)
    {
        //Since we've added the imported vertices to our own, we need to shift the indices
        unsigned int newIndex = importIndices[i] + numVertices;
        infoIndices.push_back(newIndex);
    }

    //Finally, make the surface recalculate the remaining items
    infoSurface->calculateTangents();
    infoSurface->calculateSizesAndLength();

    delete importSurface;

    rebuildInformation();
}

std::vector<unsigned int> UserInterfaceModeller::getHighlightVertexIndices()
{
    std::vector<unsigned int> highlights;
    std::vector<UIVertexPanel*>& vertexPanels = vertexInformation->getVertexPanels();
    for (auto i = vertexPanels.begin(); i != vertexPanels.end(); i++)
    {
        UIVertexPanel* vertexPanel = *i;
        if (vertexPanel->isHighlighted()) highlights.push_back(vertexPanel->index);
    }

    return highlights;
}

std::vector<unsigned int> UserInterfaceModeller::getHighlightTriangleIndices()
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
    for (unsigned int i=0; i<bones.size(); i++)
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
        glm::mat3 transposeMatrix = glm::transpose(glm::inverse(glm::mat3(transformMatrix)));

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
        for (unsigned int i=0; i<vertices.size(); i++)
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
                    newTangent += (glm::mat3(transformMatrix) * tangent) * weight;
                    newBitangent += (glm::mat3(transformMatrix) * bitangent) * weight;
                }
            }

            vertex = newVertex;
            normal = glm::normalize(newNormal);
            tangent = glm::normalize(newTangent);
            bitangent = glm::normalize(newBitangent);
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

void UserInterfaceModeller::updateVertexPosition(int index, glm::vec3 newVertexPosition)
{
    //First, get the old vertex
    std::vector<glm::vec3>& vertices = infoSurface->getVertices();
    glm::vec3 updateVertex = vertices[index];

    if (vertexInformation->updateSimilarVertices)
    {
        for (auto i=vertices.begin(); i!=vertices.end(); i++)
        {
            glm::vec3& vertexPosition = *i;
            if (vertexPosition == updateVertex)
            {
                vertexPosition = newVertexPosition;
            }
        }
    }
    else
    {
        vertices[index] = newVertexPosition;
    }
}

void UserInterfaceModeller::removeVertices(int index)
{
    std::vector<glm::vec3>& vertices = infoSurface->getVertices();

    glm::vec3 updateVertex = vertices[index];
    if (vertexInformation->updateSimilarVertices)
    {
        for (unsigned int i=0; i<vertices.size();)
        {
            if (vertices[i] == updateVertex)
            {
                removeVertex(i);
            }
            else
            {
                i++;
            }
        }
    }
    else
    {
        removeVertex(index);
    }

    infoSurface->calculateSizesAndLength();
    rebuildInformation();
}

void UserInterfaceModeller::removeVertex(unsigned int index)
{
    std::vector<glm::vec3>& vertices = infoSurface->getVertices();
    std::vector<glm::vec2>& textureCoordinates = infoSurface->getTextureCoordinates();
    std::vector<glm::vec3>& normals = infoSurface->getNormals();
    std::vector<glm::vec3>& tangents = infoSurface->getTangents();
    std::vector<glm::vec3>& bitangents = infoSurface->getBitangents();
    std::vector<glm::vec4>& boneIndices = infoSurface->getBoneIndicesAndWeights();

    vertices.erase(vertices.begin() + index);
    textureCoordinates.erase(textureCoordinates.begin() + index);
    normals.erase(normals.begin() + index);
    tangents.erase(tangents.begin() + index);
    bitangents.erase(bitangents.begin() + index);
    boneIndices.erase(boneIndices.begin() + index);

    //Remove any triangles that have this vertex
    std::vector<unsigned int>& indices = infoSurface->getIndices();
    for (unsigned int i=0; i<indices.size();)
    {
        if (std::find(indices.begin() + i, indices.begin() + i + 3, index) != indices.begin() + i + 3)
        {
            indices.erase(indices.begin() + i, indices.begin() + i + 3);
        }
        else
        {
            i+=3;
        }
    }

    //Update remaining indices greater than the removed vertex
    for (unsigned int i=0; i<indices.size(); i++)
    {
        if (indices[i] > index) indices[i]--;
    }
}

void UserInterfaceModeller::renderTextureCoordinates()
{
    textureInformation->renderTextureCoordinates();
}