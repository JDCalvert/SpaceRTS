#version 430 core

layout(location = 0) in vec3 vertexPosition;

uniform float aspectRatio;
//uniform vec2 basePosition;

void main()
{
    gl_Position = vec4
    (
        ((vertexPosition.x) * 2 / aspectRatio) - 1,
        1 - (vertexPosition.y) * 2,
        0.0,
        1.0
    );
    /*gl_Position = vec4
    (
        ((vertexPosition.x + basePosition.x) * 2 / aspectRatio) - 1,
        1 - (vertexPosition.y + basePosition.y) * 2,
        0.0,
        1.0
    );*/
}