#version 450 core

in vec4 vertexPosition;

void main()
{
	gl_Position = vertexPosition;
}