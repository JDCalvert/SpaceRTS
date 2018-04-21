#version 430 core

in vec2 textureCoordinate;

out vec3 color;

uniform sampler2D renderedTexture;

void main()
{
    color = texture(renderedTexture, textureCoordinate).rgb;
}