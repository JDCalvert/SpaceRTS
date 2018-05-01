#version 430 core

layout(location = 0) in vec2 vertexPosition;

uniform float aspectRatio;

void main()
{
    gl_Position = vec4
    (
        (vertexPosition.x * 2 / aspectRatio) - 1,
        1 - vertexPosition.y * 2,
        0.0,
        1.0
    );
}