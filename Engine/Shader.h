#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

#include "ResourceLoader.h"

class OpenGLContext;

struct ShaderInfo
{
    GLenum type;
    const char* fileName;
    GLuint shaderId;
};

class Shader
{
public:
    Shader(OpenGLContext* glContext);

    virtual void initialise() = 0;
    void initialiseForScreenSize();
    void initialiseFrame();

    static GLuint loadShaders(ShaderInfo shaders[], int numShaders);
    static GLboolean getAndSetGlCapability(GLenum cap, GLboolean enable);
    static void setGlCapability(GLenum cap, GLboolean enable);

protected:
    OpenGLContext* glContext;

    GLuint programId;

    void bindArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void enableVertexAttribute(GLuint attribId, GLuint bufferId, int attribSize);

private:
    static void compileAndCheckShader(const char* path, std::string &code, GLuint shaderId);
    static void linkAndCheckProgram(GLuint programId);
};

#endif