#version 430 core

layout(location = 0) in vec2 vertexPosition;

out vec2 textureCoordinate;

void main()
{
    gl_Position = vec4(vertexPosition, 0, 1);
    textureCoordinate.x = (vertexPosition.x + 1) / 2;
    textureCoordinate.y = (vertexPosition.y + 1) / 2;
}