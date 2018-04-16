#version 450 core

in vec4 vertexPosition;

uniform mat4 modelMatrix;
uniform mat4 modelViewProjMatrix;
uniform mat4 shieldMatrix;

out vec3 worldPosition;
out vec3 worldNormal;

out vec3 shipPosition;

void main()
{
	gl_Position = modelViewProjMatrix * vertexPosition;

	worldPosition = (modelMatrix * vertexPosition).xyz;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	worldNormal = normalMatrix * vertexPosition.xyz;

	shipPosition = (shieldMatrix * vertexPosition).xyz;
}