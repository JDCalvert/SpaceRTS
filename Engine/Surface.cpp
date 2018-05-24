#include "Surface.h"

#include <fstream>

#include <glm\gtx\transform.hpp>

Surface::Surface()
{
}

void Surface::setTextures(GLuint diffuseMap, GLuint normalMap, GLuint specularMap)
{
	this->diffuseMap = diffuseMap;
	this->normalMap = normalMap;
	this->specularMap = specularMap;
}

void Surface::setAmbientColour(glm::vec4 ambientColour)
{
	this->ambientColour = ambientColour;
}

void Surface::setDiffuseColour(glm::vec4 diffuseColour)
{
	this->diffuseColour = diffuseColour;
}

void Surface::loadFromFile(const char* objFilePath, float scale)
{
	loadFromFile(objFilePath);

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i] *= scale;
	}

	for (unsigned int i = 0; i < bones.size(); i++)
	{
		Bone& bone = bones[i];
		bone.relative[3][0] *= scale;
		bone.relative[3][1] *= scale;
		bone.relative[3][2] *= scale;
	}
}

void Surface::loadFromFile(const char* objFilePath)
{
	std::vector<glm::mat4> boneRelatives;
	std::vector<int> boneParents;
	
    bool success = loadObj(objFilePath, boneRelatives, boneParents);
    if (success)
    {
        bones.clear();

	    for (unsigned int i=0; i<boneRelatives.size(); i++)
	    {
		    const glm::mat4 boneRelative = boneRelatives[i];
		    int boneParent = boneParents[i];

		    bones.push_back(Bone(boneRelative, boneParent));
	    }

	    calculateSizesAndLength();
	    prepareBones();
    }
}

bool Surface::loadObj(const char* path, std::vector<glm::mat4> &bones, std::vector<int> &parents)
{
    //Open the file as a binary, and start at the end so we can get the length, then go back to the beginning.
    std::ifstream in(path, std::ios::in | std::ios::binary | std::ios::ate);
    unsigned int fileSize = (unsigned int)in.tellg();
    in.seekg(0, std::ios::beg);

    if (!in) return false;

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
    boneIndicesAndWeights.resize(lengths[0]);
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
    memcpy(&boneIndicesAndWeights[0], data + boneIndicesOffset, boneIndicesSize);
    memcpy(&indices[0], data + indicesOffset, indicesSize);
    memcpy(&bones[0], data + bonesOffset, bonesSize);
    memcpy(&parents[0], data + parentsOffset, parentsSize);

    //Clean up
    delete lengths;
    delete data;

    return true;
}

void Surface::writeToFile(const char* fileName)
{
    calculateTangents();

    unsigned int numBones = bones.size();

    std::vector<glm::mat4> boneRelatives(numBones);
    std::vector<int> boneParents(numBones);
    for (unsigned int i=0; i<numBones; i++)
    {
        Bone bone = bones[i];
        boneRelatives[i]= bone.relative;
        boneParents[i] = bone.parent;
    }

    std::vector<unsigned int> sizes = {vertices.size(), indices.size(), bones.size()};

    int sizesSize = sizes.size() * sizeof(unsigned int);
    int boneRelativesSize = bones.size() * sizeof(glm::mat4);
    int boneParentsSize = bones.size() * sizeof(int);

    int verticesOffset = sizesSize;
    int textureCoordinatesOffset = verticesOffset + verticesSize;
    int normalsOffset = textureCoordinatesOffset + textureCoordinatesSize;
    int tangentsOffset = normalsOffset + normalsSize;
    int bitangentsOffset = tangentsOffset + tangentsSize;
    int boneIndicesOffset = bitangentsOffset + bitangentsSize;
    int indicesOffset = boneIndicesOffset + boneIndicesSize;
    int bonesOffset = indicesOffset + indicesSize;
    int parentsOffset = bonesOffset + boneRelativesSize;
    int totalSize = parentsOffset + boneParentsSize;

    char* data = new char[totalSize];
    memcpy(&data[0], &sizes[0], sizesSize);
    memcpy(&data[verticesOffset], verticesPointer, verticesSize);
    memcpy(&data[textureCoordinatesOffset], textureCoordinatesPointer, textureCoordinatesSize);
    memcpy(&data[normalsOffset], normalsPointer, normalsSize);
    memcpy(&data[tangentsOffset], tangentsPointer, tangentsSize);
    memcpy(&data[bitangentsOffset], bitangentsPointer, bitangentsSize);
    memcpy(&data[boneIndicesOffset], boneIndicesPointer, boneIndicesSize);
    memcpy(&data[indicesOffset], indicesPointer, indicesSize);
    memcpy(&data[bonesOffset], &boneRelatives[0], boneRelativesSize);
    memcpy(&data[parentsOffset], &boneParents[0], boneParentsSize);

    std::ofstream objFile(fileName, std::ios::out | std::ios::binary);
    objFile.write(data, totalSize);
    objFile.close();
}

void Surface::calculateNormals()
{
    //Reset to zero
    unsigned int numVertices = vertices.size();
    normals.resize(numVertices);
    std::fill(normals.begin(), normals.end(), glm::vec3());

    //Loop through the triangles, work out the normal, and ADD this to the three vertices
    //If a vertex is in more than one triangle, the normals will be added together
    for (unsigned int i=0; i<indices.size(); i+=3)
    {
        int index0 = indices[i];
        int index1 = indices[i+1];
        int index2 = indices[i+2];

        glm::vec3 vertex0 = vertices[index0];
        glm::vec3 vertex1 = vertices[index1];
        glm::vec3 vertex2 = vertices[index2];

        glm::vec3 deltaPos1 = vertex1 - vertex0;
        glm::vec3 deltaPos2 = vertex2 - vertex0;

        glm::vec3 normal = glm::cross(deltaPos1, deltaPos2);
        normal /= glm::length(normal);

        normals[index0] += normal;
        normals[index1] += normal;
        normals[index2] += normal;
    }

    //Now, normalise to length 1
    for (unsigned int i=0; i<numVertices; i++)
    {
        glm::vec3& normal = normals[i];
        normal /= glm::length(normal);
    }
}

void Surface::calculateTangents()
{
    tangents.clear();
    bitangents.clear();

    unsigned int numVertices = vertices.size();
    tangents.resize(numVertices);
    bitangents.resize(numVertices);

    for (unsigned int i=0; i<indices.size(); i+=3)
    {
        int index0 = indices[i];
        int index1 = indices[i + 1];
        int index2 = indices[i + 2];

        glm::vec3 vertex0 = vertices[index0];
        glm::vec3 vertex1 = vertices[index1];
        glm::vec3 vertex2 = vertices[index2];

        glm::vec3 deltaPos1 = vertex1 - vertex0;
        glm::vec3 deltaPos2 = vertex2 - vertex0;

        glm::vec2 texCoord0 = textureCoordinates[index0];
        glm::vec2 texCoord1 = textureCoordinates[index1];
        glm::vec2 texCoord2 = textureCoordinates[index2];

        glm::vec2 deltaTextureCoordinate1 = texCoord1 - texCoord0;
        glm::vec2 deltaTextureCoordinate2 = texCoord2 - texCoord0;

        float r = 1.0f / (deltaTextureCoordinate1.x * deltaTextureCoordinate2.y - deltaTextureCoordinate1.y * deltaTextureCoordinate2.x);
        glm::vec3 tangent = (deltaPos1 * deltaTextureCoordinate2.y - deltaPos2 * deltaTextureCoordinate1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaTextureCoordinate1.x - deltaPos1 * deltaTextureCoordinate2.x) * r;

        tangents[index0] += tangent;
        tangents[index1] += tangent;
        tangents[index2] += tangent;
        bitangents[index0] += bitangent;
        bitangents[index1] += bitangent;
        bitangents[index2] += bitangent;
    }

    for (unsigned int i=0; i<tangents.size(); i++)
    {
        tangents[i] = glm::normalize(tangents[i]);
        bitangents[i] = glm::normalize(bitangents[i]);
    }
}

void Surface::setUpColourPointers()
{
	ambientPointer = &ambientColour[0];
	diffusePointer = &diffuseColour[0];
}

void Surface::calculateSizesAndLength()
{
	length = indices.size();

	verticesSize = vertices.size() * sizeof(glm::vec3);
	textureCoordinatesSize = textureCoordinates.size() * sizeof(glm::vec2);
	normalsSize = normals.size() * sizeof(glm::vec3);
	tangentsSize = tangents.size() * sizeof(glm::vec3);
	bitangentsSize = bitangents.size() * sizeof(glm::vec3);
	boneIndicesSize = boneIndicesAndWeights.size() * sizeof(glm::vec4);
	indicesSize = length * sizeof(unsigned int);

	if (vertices.size() > 0) verticesPointer = &vertices[0];
	if (textureCoordinates.size() > 0) textureCoordinatesPointer = &textureCoordinates[0];
	if (normals.size() > 0) normalsPointer = &normals[0];
	if (tangents.size() > 0) tangentsPointer = &tangents[0];
	if (bitangents.size() > 0) bitangentsPointer = &bitangents[0];
	if (indices.size() > 0) indicesPointer = &indices[0];
	if (boneIndicesAndWeights.size() > 0) boneIndicesPointer = &boneIndicesAndWeights[0];
}

void Surface::prepareBones()
{
	//All the bones are in their bind position, work out their absolute position from their relative positions, and calculate the inverse of this.
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		Bone& bone = bones[i];
		int index = bone.parent;
		if (index == -1)
		{
			bone.absolute = bone.relative;
		}
		else
		{
			bone.absolute = bones[index].absolute * bone.relative;
		}
		bone.inverseBind = glm::inverse(bone.absolute);
		boneMatrices.push_back(bone.absolute * bone.inverseBind);
	}

	boneMatricesSize = bones.size() * sizeof(glm::mat4);
	if (boneMatricesSize > 0) boneMatricesPointer = &boneMatrices[0];
}

void Surface::recalculateModelBoneMatrices()
{
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		Bone& bone = bones[i];
		int index = bone.parent;
		if (index == -1)
		{
			bone.absolute = bone.relative;
		}
		else
		{
			bone.absolute = bones[index].absolute * bone.relative;
		}
		boneMatricesPointer[i] = bone.absolute * bone.inverseBind;
	}
}

std::vector<glm::vec3>& Surface::getVertices()
{
    return vertices;
}
std::vector<glm::vec2>& Surface::getTextureCoordinates()
{
    return textureCoordinates;
}
std::vector<glm::vec3>& Surface::getNormals()
{
    return normals;
}
std::vector<glm::vec3>& Surface::getTangents()
{
    return tangents;
}
std::vector<glm::vec3>& Surface::getBitangents()
{
    return bitangents;
}
std::vector<unsigned int>& Surface::getIndices()
{
    return indices;
}
std::vector<glm::vec4>& Surface::getBoneIndicesAndWeights()
{
    return boneIndicesAndWeights;
}
std::vector<Bone>& Surface::getBones()
{
    return bones;
}