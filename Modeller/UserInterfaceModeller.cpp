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

    saveLoadPanel = new UISaveLoadPanel();
    saveLoadPanel->build();
    addComponent(saveLoadPanel);

    importPanel = new UIImportPanel();
    importPanel->build();
    addComponent(importPanel);

    textureInformation = new UITextureInformation(infoSurface);
    textureInformation->build();
    addComponent(textureInformation);

    vertexInformation = new UIVertexInformation(infoSurface);
    addComponent(vertexInformation);

    triangleInformation = new UITriangleInformation(infoSurface);
    addComponent(triangleInformation);

    boneInformation = new UIBoneInformation(infoSurface);
    addComponent(boneInformation);

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

    textureInformation->updateTextures();
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

    std::vector<Bone>& infoBones = infoSurface->bones;
    int numBones = infoBones.size();

    //Second, append the new surface's bones onto our surface
    //Ignore the first bone, as we'll use our active bone as the base
    for (unsigned int i=1; i<importSurface->bones.size(); i++)
    {
        Bone newBone = importSurface->bones[i];
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

    int numVertices = infoSurface->vertices.size();

    glm::vec2 baseTexCoord = importPanel->textureCoordinatesTopLeft;
    glm::vec2 scaleTexCoord = importPanel->textureCoordinatesBottomRight - baseTexCoord;

    //Third, add all the vertices, transformed by the active bone matrix
    for (unsigned int i=0; i<importSurface->vertices.size(); i++)
    {
        glm::vec4 newVertex = infoBones[activeBone].absolute * glm::vec4(importSurface->vertices[i], 1.0f);
        glm::vec3 newNormal = transposeMatrices[activeBone] * importSurface->normals[i];
        glm::vec4 newBoneIndices = importSurface->boneIndicesAndWeights[i];

        glm::vec2 importTextureCoordinate = importSurface->textureCoordinates[i];
        glm::vec2 newTextureCoordinate = baseTexCoord + glm::matrixCompMult(importTextureCoordinate, scaleTexCoord);

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

        infoSurface->vertices.push_back(newVertex);
        infoSurface->textureCoordinates.push_back(newTextureCoordinate);
        infoSurface->normals.push_back(newNormal);
        infoSurface->boneIndicesAndWeights.push_back(newBoneIndices);
    }

    //Next, add all the triangles from the import surface to ours
    for (unsigned int i=0; i<importSurface->indices.size(); i++)
    {
        //Since we've added the imported vertices to our own, we need to shift the indices
        unsigned int newIndex = importSurface->indices[i] + numVertices;
        infoSurface->indices.push_back(newIndex);
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

std::vector<unsigned int> UserInterfaceModeller::getHighlightBoneIndices()
{
    std::vector<unsigned int> highlights;
    for (UIBonePanel* bonePanel : boneInformation->getBonePanels())
    {
        if (bonePanel->isHighlighted())
        {
            unsigned int index = bonePanel->getIndex();
            highlights.push_back(index);
        }
    }
    return highlights;
}

void UserInterfaceModeller::recalculateVertexPositions()
{
    //Update the absolute positions of all the bones
    std::vector<Bone>& bones = infoSurface->bones;
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
        for (unsigned int i=0; i<infoSurface->vertices.size(); i++)
        {
            glm::vec3& vertex = infoSurface->vertices[i];
            glm::vec3& normal = infoSurface->normals[i];
            glm::vec3& tangent = infoSurface->tangents[i];
            glm::vec3& bitangent = infoSurface->bitangents[i];
        
            glm::vec3 newVertex;
            glm::vec3 newNormal;
            glm::vec3 newTangent;
            glm::vec3 newBitangent;

            for (unsigned int j=0; j<2; j++)
            {
                glm::vec4& boneIndicesAndWeights = infoSurface->boneIndicesAndWeights[i];
                int parentIndex = boneIndicesAndWeights[j*2];
                float weight = boneIndicesAndWeights[j*2+1];
            
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
    Bone& bone = infoSurface->bones[activeBone];
    glm::mat4 transform = bone.absolute;
    glm::mat3 transpose = glm::transpose(glm::mat3(transform));

    glm::vec3 vertexPosition(transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    glm::vec2 vertexTextureCoordinates(0.0f, 0.0f);
    glm::vec3 vertexNormal(transpose * glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec4 boneIndexAndWeight(activeBone, 1.0f, -1, 0.0f);

    infoSurface->vertices.push_back(vertexPosition);
    infoSurface->textureCoordinates.push_back(vertexTextureCoordinates);
    infoSurface->normals.push_back(vertexNormal);
    infoSurface->boneIndicesAndWeights.push_back(boneIndexAndWeight);

    infoSurface->calculateTangents();
    infoSurface->calculateSizesAndLength();
}

void UserInterfaceModeller::newBone()
{
    unsigned int activeBone = boneInformation->activeBone;
    Bone newBone(glm::mat4(), activeBone);

    infoSurface->bones.push_back(newBone);

    infoSurface->prepareBones();
}

void UserInterfaceModeller::updateVertexPosition(int index, glm::vec3 newVertexPosition)
{
    //First, get the old vertex
    glm::vec3 updateVertex = infoSurface->vertices[index];

    if (vertexInformation->updateSimilarVertices)
    {
        for (glm::vec3& vertexPosition : infoSurface->vertices)
        {
            if (vertexPosition == updateVertex)
            {
                vertexPosition = newVertexPosition;
            }
        }
    }
    else
    {
        infoSurface->vertices[index] = newVertexPosition;
    }
}

void UserInterfaceModeller::removeVertices(int index)
{
    std::vector<glm::vec3>& vertices = infoSurface->vertices;

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
    infoSurface->vertices.erase(infoSurface->vertices.begin() + index);
    infoSurface->textureCoordinates.erase(infoSurface->textureCoordinates.begin() + index);
    infoSurface->normals.erase(infoSurface->normals.begin() + index);
    infoSurface->tangents.erase(infoSurface->tangents.begin() + index);
    infoSurface->bitangents.erase(infoSurface->bitangents.begin() + index);
    infoSurface->boneIndicesAndWeights.erase(infoSurface->boneIndicesAndWeights.begin() + index);

    //Remove any triangles that have this vertex
    std::vector<unsigned int>& indices = infoSurface->indices;
    unsigned int numTriangles = indices.size() / 3;
    for (unsigned int i=0; i<numTriangles;)
    {
        auto firstIndex = indices.begin() + i * 3;
        if (std::find(firstIndex, firstIndex + 3, index) != firstIndex + 3)
        {
            removeTriangle(i);
            numTriangles--;
        }
        else
        {
            i++;
        }
    }

    //Update remaining indices greater than the removed vertex
    for (unsigned int i=0; i<indices.size(); i++)
    {
        if (indices[i] > index) indices[i]--;
    }
}

void UserInterfaceModeller::removeTriangle(unsigned int triangleIndex)
{
    std::vector<unsigned int>& indices = infoSurface->indices;
    auto firstIndexRemove = indices.begin() + triangleIndex * 3;
    indices.erase(firstIndexRemove, firstIndexRemove + 3);

    infoSurface->calculateSizesAndLength();
    rebuildInformation();
}

void UserInterfaceModeller::removeBone(unsigned int index)
{
    for (unsigned int i=0; i<infoSurface->boneIndicesAndWeights.size(); i++)
    {
        glm::vec4& boneIndex = infoSurface->boneIndicesAndWeights[i];
        if (boneIndex[0] == index)
        {
            //If the vertex is partially dependent on another bone, shift the dependency entirely to that
            if (boneIndex[2] > -1)
            {
                boneIndex[0] = boneIndex[2];
                boneIndex[1] = 1.0f;
                boneIndex[2] = -1;
                boneIndex[3] = 0.0f;
            }
            //Otherwise remove the bone
            else
            {
                removeVertex(i);
                i--;
            }
        }
        else if (boneIndex[2] == index)
        {
            //If the vertex is partially dependent on this bone then move the dependency entirely to the other one
            boneIndex[1] = 1.0f;
            boneIndex[2] = -1;
            boneIndex[3] = 0.0f;
        }

        if (boneIndex[0] > index) boneIndex[0]--;
        if (boneIndex[2] > index) boneIndex[2]--;
    }

    for (unsigned int i=0; i<infoSurface->bones.size(); i++)
    {
        Bone& bone = infoSurface->bones[i];
        if (bone.parent == index)
        {
            //Remove the bone if it's dependent on the one being removed
            removeBone(i);
        }
        else if (bone.parent > index)
        {
            //If the bone's parent is above the one being removed, then reduce it
            bone.parent--;
        }
    }

    infoSurface->bones.erase(infoSurface->bones.begin() + index);

    infoSurface->calculateSizesAndLength();
    rebuildInformation();
}

void UserInterfaceModeller::renderTextureCoordinates()
{
    textureInformation->renderTextureCoordinates();
}