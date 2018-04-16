#version 330 core

in vec2 position;
in vec2 dimensions;
in vec4 barColour;

uniform float screenRatio;

out vec2 dimension;
out vec4 colour;

void main()
{
	float x = position.x * (2.0 / screenRatio) - 1;
	float y = -position.y * 2 + 1;

    gl_Position = vec4(x, y, 0.0, 1.0);
    dimension = dimensions * 2;
    colour = barColour;
}
