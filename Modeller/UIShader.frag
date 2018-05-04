#version 430 core

in vec2 textureCoordinate;

uniform sampler2D textureMap;

out vec4 colour;

void main()
{
    colour = texture(textureMap, textureCoordinate);
}