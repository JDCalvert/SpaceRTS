#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include "Shader.h"

class Surface;
class Renderer;

class SimpleShader: public Shader
{
public:

    SimpleShader(Renderer* renderer);
    
    void initialise();
    void renderSurface(Surface* surface, glm::mat4 modelMatrix);

private:

    Renderer* renderer;

    //Our vertex array
    GLuint vertexArrayId;

    //Attribute array IDs
    GLuint vertexPositionId;
    GLuint vertexTextureCoordinateId;

    //Uniform IDs
    GLuint diffuseMapId;
    GLuint modelViewProjectionMatrixId;

    //Buffers
    GLuint vertexPositionBufferId;
    GLuint vertexTextureCoordinateBufferId;
    GLuint indicesBufferId;
};

#endif