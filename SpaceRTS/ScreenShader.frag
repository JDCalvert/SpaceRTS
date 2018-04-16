#version 330 core

in vec2 textureCoord;

uniform sampler2D renderedTexture;

out vec4 color;

void main()
{
	color = texture(renderedTexture, textureCoord);
}