#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

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
    virtual void initialise() = 0;
    void initialiseForScreenSize();
    void initialiseFrame();

    void loadShaders(ShaderInfo shaders[], int numShaders);

protected:
    GLuint programId;

    void bindArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void enableVertexAttribute(GLuint attribId, GLuint bufferId, int attribSize);

private:
    void compileAndCheckShader(const char* path, std::string &code, GLuint shaderId);
    void linkAndCheckProgram(GLuint programId);
    std::string readFromFile(const char* path);
};

#endif