#version 330 core

in vec4 vertexPosition;
out vec2 textureCoord;

void main()
{
	gl_Position = vertexPosition;
	textureCoord = (vertexPosition.xy + 1.0) / 2.0;
}