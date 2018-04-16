#version 450

in vec4 vertexPosition;
in vec2 vertexTextureCoordinates;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;
in vec4 vertexBones;

out vec3 worldPosition;
out vec3 worldNormal;
out vec3 worldTangent;
out vec3 worldBitangent;
out vec2 textureCoordinates;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

layout (std430, binding = 0) buffer boneBuffer
{
	mat4 bones[];
};

void main()
{
	vec4 vertex = vec4(0, 0, 0, 0);
	vec3 normal = vec3(0, 0, 0);
	vec3 tangent = vec3(0, 0, 0);
	vec3 bitangent = vec3(0, 0, 0);

	for (int i=0; i<2; i++)
	{
		int boneIndex = int(vertexBones[i*2]);
		float boneWeight = vertexBones[i*2+1];
		
		if (boneIndex > -1)
		{
			mat4 bone = bones[boneIndex];
			mat3 boneNormal = transpose(mat3(inverse(bone)));

			vertex = ((bone * vertexPosition) * boneWeight) + vertex;
			normal = ((boneNormal * vertexNormal) * boneWeight) + normal;
			tangent = ((boneNormal * vertexTangent) * boneWeight) + tangent;
			bitangent = ((boneNormal * vertexBitangent) * boneWeight) + bitangent;
		}
	}

	//The vertex's position on the screen
	gl_Position = modelViewProjectionMatrix * vertex;

	//The vertex's position in world space. We don't need this to stay in homogeneous coordinates
	worldPosition = (modelMatrix * vertex).xyz;

	//Use the inverse transpose of the top 3x3 matrix from the model matrix to transform the normals into world coordinates
	mat3 normalModelMatrix = transpose(mat3(inverse(modelMatrix)));
	worldNormal = normalModelMatrix * normal;
	worldTangent = normalModelMatrix * tangent;
	worldBitangent = normalModelMatrix * bitangent;

	//Just pass the texture coordinates through
	textureCoordinates = vertexTextureCoordinates;
}