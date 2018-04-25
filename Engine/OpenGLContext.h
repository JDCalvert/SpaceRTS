#ifndef OPEN_GL_CONTEXT_H
#define OPEN_GL_CONTEXT_H

#include <unordered_map>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

class Renderer;

class OpenGLContext
{
private:
    std::unordered_map<GLenum, GLboolean> glCapabilities;
    void actuallySetEnabled(GLenum glCapability, GLboolean enabled);

    std::vector<Renderer*> renderers;

    GLsizei width, height;

public:
    void setEnabled(GLenum glCapability, GLboolean enabled);
    void addRenderer(Renderer* renderer);
    void resize(int width, int height);

    
};

#endif