#include "Shader.h"
#include "ResourceLoader.h"
#include "OpenGLContext.h"

void Shader::initialiseForScreenSize()
{
}

void Shader::initialiseFrame()
{
}

void Shader::bindArrayBufferData(GLuint bufferId, int size, void* dataPointer)
{
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, size, dataPointer, GL_STATIC_DRAW);
}

void Shader::bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, dataPointer, GL_STATIC_DRAW);
}

void Shader::enableVertexAttribute(GLuint attribId, GLuint bufferId, int attribSize)
{
    glEnableVertexAttribArray(attribId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glVertexAttribPointer(attribId, attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

/**
 * Create and link a shader program with various shaders defined in the input
 */
GLuint Shader::loadShaders(ShaderInfo shaders[], int numShaders)
{
    //Create the program
    GLuint programId = glCreateProgram();

    //Compile and attach each shader to the program
    for (int i = 0; i<numShaders; i++)
    {
        ShaderInfo shader = shaders[i];
        GLenum shaderType = shader.type;
        const char* shaderPath = shader.fileName;

        //Create the shader
        GLuint shaderId = glCreateShader(shaderType);
        shader.shaderId = shaderId;

        //Compile the shader
        std::string shaderCode = ResourceLoader::readFromFile(shaderPath);
        compileAndCheckShader(shaderPath, shaderCode, shaderId);

        glAttachShader(programId, shaderId);
    }

    linkAndCheckProgram(programId);

    //Delete the shaders, we don't need them anymore
    for (int i = 0; i<numShaders; i++)
    {
        ShaderInfo shader = shaders[i];
        GLuint shaderId = shader.shaderId;
        glDeleteShader(shaderId);
    }

    return programId;
}

void Shader::compileAndCheckShader(const char* path, std::string &code, GLuint shaderId)
{
    GLint result = GL_FALSE;
    int infoLogLength;

    //Compile the shader
    printf("Compiling shader: %s\n", path);
    char const* vertexSourcePointer = code.c_str();
    glShaderSource(shaderId, 1, &vertexSourcePointer, NULL);
    glCompileShader(shaderId);

    //Check the shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> errorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }
}

void Shader::linkAndCheckProgram(GLuint programId)
{
    glLinkProgram(programId);

    //Check the program
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
        printf("%s\n", &programErrorMessage[0]);
    }
}

GLboolean Shader::getAndSetGlCapability(GLenum cap, GLboolean enable)
{
    GLboolean isEnabled = glIsEnabled(cap);
    setGlCapability(cap, enable);
    return isEnabled;
}

void Shader::setGlCapability(GLenum cap, GLboolean enable)
{
    if (enable)
    {
        glEnable(cap);
    }
    else
    {
        glDisable(cap);
    }
}