#include "Surface.h"
#include "ResourceLoader.h"

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
		BindBone& bone = bones[i];
		bone.relative[3][0] *= scale;
		bone.relative[3][1] *= scale;
		bone.relative[3][2] *= scale;
	}
}

void Surface::loadFromFile(const char* objFilePath)
{
	std::vector<glm::mat4> boneRelatives;
	std::vector<int> boneParents;
	loadObj(objFilePath, boneRelatives, boneParents);

	for (unsigned int i = 0; i < boneRelatives.size(); i++)
	{
		const glm::mat4 &boneRelative = boneRelatives[i];
		int boneParent = boneParents[i];

		BindBone bone(boneRelative, boneParent);
		bones.push_back(bone);
	}

	calculateSizesAndLength();
	prepareBones();
}

void Surface::loadObj(const char* path, std::vector<glm::mat4> &bones, std::vector<int> &parents)
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
	bonesSize = boneIndicesAndWeights.size() * sizeof(glm::vec4);
	indicesSize = length * sizeof(unsigned int);

	if (vertices.size() > 0) verticesPointer = &vertices[0];
	if (textureCoordinates.size() > 0) textureCoordinatesPointer = &textureCoordinates[0];
	if (normals.size() > 0) normalsPointer = &normals[0];
	if (tangents.size() > 0) tangentsPointer = &tangents[0];
	if (bitangents.size() > 0) bitangentsPointer = &bitangents[0];
	if (indices.size() > 0) indicesPointer = &indices[0];
	if (boneIndicesAndWeights.size() > 0) bonesPointer = &boneIndicesAndWeights[0];
}

void Surface::prepareBones()
{
	//All the bones are in their bind position, work out their absolute position from their relative positions, and calculate the inverse of this.
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		BindBone &bone = bones[i];
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
	if (boneMatricesSize > 0)
	{
		boneMatricesPointer = &boneMatrices[0];
	}
}

void Surface::recalculateModelBoneMatrices()
{
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		Bone &bone = modelBonesPointer[i];
		int index = bone.parent;
		if (index == -1)
		{
			bone.absolute = bone.relative;
		}
		else
		{
			bone.absolute = modelBonesPointer[index].absolute * bone.relative;
		}
		boneMatricesPointer[i] = bone.absolute * bones[i].inverseBind;
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
std::vector<unsigned int>& Surface::getIndices()
{
    return indices;
}