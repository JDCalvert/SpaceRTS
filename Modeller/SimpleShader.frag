#version 430 core

in vec2 textureCoordinate;

out vec4 color;

uniform sampler2D diffuseTexture;

void main()
{
	color = texture(diffuseTexture, textureCoordinate);
}