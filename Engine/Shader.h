#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

#include "ResourceLoader.h"

class Shader
{
public:

    virtual void initialise() = 0;
    void initialiseForScreenSize();
    void initialiseFrame();

    static GLuint loadShaders(ShaderInfo shaders[], int numShaders);

protected:
    GLuint programId;

    void bindArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void enableVertexAttrib(GLuint attribId, GLuint bufferId, int attribSize);

private:
    static void compileAndCheckShader(const char* path, std::string &code, GLuint shaderId);
    static void linkAndCheckProgram(GLuint programId);
};

#endif