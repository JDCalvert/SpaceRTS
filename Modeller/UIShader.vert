#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;

uniform float aspectRatio;
uniform vec2 basePosition;

out vec2 textureCoordinate;

void main()
{
    gl_Position = vec4
    (
        ((vertexPosition.x + basePosition.x) * 2 / aspectRatio) - 1,
        1 - (vertexPosition.y + basePosition.y) * 2,
        0.0,
        1.0
    );

    textureCoordinate = vertexTextureCoordinate;
}