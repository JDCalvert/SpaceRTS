#version 430 core

in vec2 textureCoordinate;

out vec3 color;

uniform sampler2D diffuseTexture;

void main()
{
	color = texture(diffuseTexture, textureCoordinate).rgb;
}