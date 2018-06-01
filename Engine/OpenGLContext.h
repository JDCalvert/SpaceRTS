#ifndef OPEN_GL_CONTEXT_H
#define OPEN_GL_CONTEXT_H

#include <unordered_map>
#include <vector>
#include <queue>

#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "Event.h"

class Renderer;

class OpenGLContext
{
public:
    OpenGLContext();

    static OpenGLContext* initialiseContext();
    static OpenGLContext* currentContext();
    static void setCurrentContext(OpenGLContext* glContext);
    
    //Callbacks for GLFW events
    static void windowResized(GLFWwindow* window, int width, int height);
    static void mouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
    static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void textEvent(GLFWwindow* window, unsigned int codepoint);

    glm::vec2 getCursorPosition();

    void initialiseFrame();

    bool mouseButtonDown(int key);
    bool keyPressed(int key);

    void setEnabled(GLenum glCapability, GLboolean enabled);
    void setLineWidth(GLfloat lineWidth);
    void setPointSize(GLfloat pointSize);

    void addRenderer(Renderer* renderer);
    
    void resize(int width, int height);
    void mouseButtonEvent(int button, int action, int mods);
    void mouseScrollEvent(double xOffset, double yOffset);
    void keyEvent(int key, int scancode, int action, int mods);
    void textEvent(int codepoint);

    void bindDefaultFrameBuffer();
    void clearScreen();
    void flip();

    bool shouldClose();

    double getDeltaTime();
    GLFWwindow* getWindow();
    float getAspectRatio();
    GLsizei getWidth();
    GLsizei getHeight();

    Event* nextEvent();
    void clearKeyEvent(KeyEvent event);

    glm::dvec2 getMousePositionScreenSpace();
    glm::dvec2 getDeltaMousePosition();

private:
    static std::unordered_map<GLFWwindow*, OpenGLContext*> contextsByWindow;
    static OpenGLContext* currentGlContext;

    std::unordered_map<GLenum, GLboolean> glCapabilities;
    std::vector<Renderer*> renderers;

    GLfloat pointSize;
    GLfloat lineWidth;

    GLFWwindow* window;
    GLsizei width, height;

    glm::dvec2 mousePosition, deltaMousePosition;
    std::queue<Event*> events;

    double time, deltaTime;
};

#endif