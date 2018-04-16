#version 330 core

in vec2 vertexPosition;
in vec2 vertexTextureCoordinate;

uniform float screenRatio;

out vec2 textureCoordinates;

void main()
{
	float x = vertexPosition.x * (2.0 / screenRatio) - 1;
	float y = -vertexPosition.y * 2 + 1;

	gl_Position = vec4(x, y, 0, 1);
	textureCoordinates = vertexTextureCoordinate;
}