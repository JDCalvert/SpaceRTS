#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

#include "ResourceLoader.h"

class Renderer;

class Shader
{
public:
    Shader(Renderer* renderer);

    virtual void initialise() = 0;
    void initialiseForScreenSize();
    void initialiseFrame();

    static GLuint loadShaders(ShaderInfo shaders[], int numShaders);

protected:
    GLuint programId;
    Renderer* renderer;

private:
    static void compileAndCheckShader(const char* path, std::string &code, GLuint shaderId);
    static void linkAndCheckProgram(GLuint programId);
};

#endif