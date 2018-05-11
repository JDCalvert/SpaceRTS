#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <string>
#include <unordered_map>

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
    static std::unordered_map<std::string, Shader*> shaders;

    template <class T>
    static T* loadShader(T* shader, std::string shaderName)
    {
        shaders[shaderName] = shader;
        shader->initialise();
        return shader;
    }

    static Shader* getShader(std::string shaderName);

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