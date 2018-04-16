#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float screenRatio;

in vec2 dimension[];
in vec4 colour[];

out vec4 fragColour;

void main()
{
	vec4 position = gl_in[0].gl_Position;

	float dimensionX = dimension[0].x / screenRatio;
	float dimensionY = -dimension[0].y;

	vec4 points[4];
	points[0] = position;
	points[1] = position + vec4(	   0.0, dimensionY, 0.0, 0.0);
	points[2] = position + vec4(dimensionX,	0.0,		0.0, 0.0);
	points[3] = position + vec4(dimensionX, dimensionY, 0.0, 0.0);

	for (int i=0; i<4; i++)
	{
		gl_Position = points[i];
		fragColour = colour[0];
		EmitVertex();
	}
	EndPrimitive();
}