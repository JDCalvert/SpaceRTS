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
    static std::unordered_map<GLFWwindow*, OpenGLContext*> contextsByWindow;

    std::unordered_map<GLenum, GLboolean> glCapabilities;
    std::vector<Renderer*> renderers;

    GLFWwindow* window;
    GLsizei width, height;

    void actuallySetEnabled(GLenum glCapability, GLboolean enabled);

public:
    static OpenGLContext* initialiseNewContext();
    static void windowResized(GLFWwindow* window, int width, int height);

    void setEnabled(GLenum glCapability, GLboolean enabled);

    void addRenderer(Renderer* renderer);
    void resize(int width, int height);

    void bindDefaultFrameBuffer();
    void clearScreen();
    void flip();

    GLFWwindow* getWindow();
    GLsizei getWidth();
    GLsizei getHeight();
};

#endif