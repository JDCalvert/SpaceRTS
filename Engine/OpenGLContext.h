#ifndef OPEN_GL_CONTEXT_H
#define OPEN_GL_CONTEXT_H

#include <unordered_map>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class Renderer;

class OpenGLContext
{
private:    
    static std::unordered_map<GLFWwindow*, OpenGLContext*> contextsByWindow;
    static OpenGLContext* currentGlContext;

    std::unordered_map<GLenum, GLboolean> glCapabilities;
    std::vector<Renderer*> renderers;

    GLFWwindow* window;
    GLsizei width, height;

    glm::dvec2 mousePosition, deltaMousePosition;

    double time, deltaTime;

public:
    OpenGLContext();

    static OpenGLContext* initialiseNewContext();
    static OpenGLContext* currentContext();
    static void setCurrentContext(OpenGLContext* glContext);
    static void windowResized(GLFWwindow* window, int width, int height);

    void initialiseFrame();

    bool mouseButtonPressed(int key);
    bool keyPressed(int key);

    void setEnabled(GLenum glCapability, GLboolean enabled);

    void addRenderer(Renderer* renderer);
    void resize(int width, int height);

    void bindDefaultFrameBuffer();
    void clearScreen();
    void flip();

    double getDeltaTime();
    GLFWwindow* getWindow();
    float getAspectRatio();
    GLsizei getWidth();
    GLsizei getHeight();
    glm::dvec2 getDeltaMousePosition();
};

#endif