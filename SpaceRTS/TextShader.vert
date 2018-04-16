#version 330 core

in vec2 vertexPosition;
in vec2 vertexTextureCoordinate;

uniform vec2 textPosition;
uniform float size;
uniform float screenRatio;

out vec2 textureCoordinates;

void main()
{
	vec2 position = (vertexPosition * size) + textPosition;
	float x = position.x * (2.0 / screenRatio) - 1;
	float y = -position.y * 2 + 1;

	gl_Position = vec4(x, y, 0, 1);
	textureCoordinates = vertexTextureCoordinate;
}