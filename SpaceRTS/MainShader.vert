#version 450 core

in vec3 vertexPosition;
in vec2 vertexTextureCoordinates;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;

out vec3 worldPosition;
out vec3 worldNormal;
out vec3 worldTangent;
out vec3 worldBitangent;
out vec2 textureCoordinates;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

void main()
{
	vec4 vertexPosition4 = vec4(vertexPosition, 1.0);
	vec4 vertexNormal4	 = vec4(normalize(vertexNormal), 0.0);
	vec4 vertexTangent4  = vec4(normalize(vertexTangent), 0.0);
	vec4 vertexBitangent4 = vec4(normalize(vertexBitangent), 0.0);

	gl_Position = modelViewProjectionMatrix * vertexPosition4;

	worldPosition = (modelMatrix * vertexPosition4).xyz;
	worldNormal = (modelMatrix * vertexNormal4).xyz;
	worldTangent = (modelMatrix * vertexTangent4).xyz;
	worldBitangent = (modelMatrix * vertexBitangent4).xyz;

	textureCoordinates = vertexTextureCoordinates;
}