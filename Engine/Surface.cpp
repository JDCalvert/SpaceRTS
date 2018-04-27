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
	ResourceLoader::loadObj(objFilePath, vertices, textureCoordinates, normals, tangents, bitangents, boneIndicesAndWeights, indices, boneRelatives, boneParents);

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

void Surface::setVertices(std::vector<glm::vec3>* vertices)
{
    this->vertices = *vertices;
}
void Surface::setTextureCoordinates(std::vector<glm::vec2>* textureCoordinates)
{
    this->textureCoordinates = *textureCoordinates;
}
void Surface::setIndices(std::vector<unsigned int>* indices)
{
    this->indices = *indices;
}

std::vector<unsigned int>& Surface::getIndices()
{
    return indices;
}