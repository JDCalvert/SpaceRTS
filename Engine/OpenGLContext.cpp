#include "OpenGLContext.h"
#include "Renderer.h"

//Keep a map of contexts by their window, so we can resize the correct
//context when the window changes size.
std::unordered_map<GLFWwindow*, OpenGLContext*> OpenGLContext::contextsByWindow;
OpenGLContext* OpenGLContext::currentGlContext;

void OpenGLContext::windowResized(GLFWwindow* window, int width, int height)
{
    OpenGLContext* context = contextsByWindow[window];
    context->resize(width, height);
}

void OpenGLContext::mouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
    OpenGLContext* context = contextsByWindow[window];
    context->mouseButtonEvent(button, action, mods);
}

void OpenGLContext::mouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
    OpenGLContext* context = contextsByWindow[window];
    context->mouseScrollEvent(xOffset, yOffset);
}

void OpenGLContext::textEvent(GLFWwindow* window, unsigned int codepoint)
{
    OpenGLContext* context = contextsByWindow[window];
    context->textEvent(codepoint);
}

void OpenGLContext::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    OpenGLContext* context = contextsByWindow[window];
    context->keyEvent(key, scancode, action, mods);
}

OpenGLContext* OpenGLContext::currentContext()
{
    return currentGlContext;
}

OpenGLContext* OpenGLContext::initialiseContext()
{
    int width = 800;
    int height = 600;

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialise GLFW");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.");
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialise GLEW");
        return nullptr;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    glfwSetWindowSizeCallback(window, windowResized);
    glfwSetMouseButtonCallback(window, mouseButtonEvent);
    glfwSetScrollCallback(window, mouseScrollEvent);
    glfwSetKeyCallback(window, keyEvent);
    glfwSetCharCallback(window, textEvent);

    OpenGLContext* context = new OpenGLContext();
    context->window = window;
    context->width = width;
    context->height = height;

    setCurrentContext(context);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    contextsByWindow[window] = context;

    return context;
}

void OpenGLContext::setCurrentContext(OpenGLContext* glContext)
{
    currentGlContext = glContext;
    glfwMakeContextCurrent(glContext->window);
}

OpenGLContext::OpenGLContext()
{
    time = glfwGetTime();
}

void OpenGLContext::initialiseFrame()
{
    double previousTime = time;
    time = glfwGetTime();
    deltaTime = time - previousTime;

    glm::dvec2 previousMousePosition = mousePosition;
    glfwGetCursorPos(window, &mousePosition.x, &mousePosition.y);
    deltaMousePosition = mousePosition - previousMousePosition;
}

void OpenGLContext::bindDefaultFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
}

void OpenGLContext::clearScreen()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLContext::flip()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void OpenGLContext::setEnabled(GLenum glCapability, GLboolean enabled)
{
    auto it = glCapabilities.find(glCapability);
    if (it == glCapabilities.end() || it->second != enabled)
    {
        enabled ? glEnable(glCapability) : glDisable(glCapability);
        glCapabilities[glCapability] = enabled;
    }
}

void OpenGLContext::setLineWidth(GLfloat newLineWidth)
{
    if (newLineWidth != lineWidth)
    {
        lineWidth = newLineWidth;
        glLineWidth(lineWidth);
    }   
}

void OpenGLContext::setPointSize(GLfloat newPointSize)
{
    if (newPointSize != pointSize)
    {
        pointSize = newPointSize;
        glPointSize(pointSize);
    }
}

void OpenGLContext::addRenderer(Renderer* renderer)
{
    renderer->resize(width, height);
    renderers.push_back(renderer);
}

void OpenGLContext::resize(int width, int height)
{
    this->width = width;
    this->height = height;

    for (auto i = renderers.begin(); i != renderers.end(); i++)
    {
        Renderer* renderer = *i;
        renderer->resize(width, height);
    }
}

bool OpenGLContext::shouldClose()
{
    return keyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(window);
}

void OpenGLContext::mouseButtonEvent(int button, int action, int mods)
{
    MouseClickEvent* mouseEvent = new MouseClickEvent {getCursorPosition(), button, action};
    events.push(mouseEvent);
}

void OpenGLContext::mouseScrollEvent(double xOffset, double yOffset)
{
    MouseScrollEvent* mouseEvent = new MouseScrollEvent {getCursorPosition(), xOffset, yOffset};
    events.push(mouseEvent);
}

glm::vec2 OpenGLContext::getCursorPosition()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glm::vec2 cursorPosition(xpos, ypos);
    return cursorPosition /= height;
}

void OpenGLContext::keyEvent(int key, int scancode, int action, int mods)
{
    KeyEvent* keyEvent = new KeyEvent {key, scancode, action, mods};
    events.push(keyEvent);
}

void OpenGLContext::textEvent(int codepoint)
{
    TextEvent* keyEvent = new TextEvent {codepoint};
    events.push(keyEvent);
}

Event* OpenGLContext::nextEvent()
{
    if (events.size() == 0) return nullptr;

    Event* event = events.front();
    events.pop();

    return event;
}

bool OpenGLContext::mouseButtonDown(int key)
{
    return glfwGetMouseButton(window, key) == GLFW_PRESS;
}

bool OpenGLContext::keyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void OpenGLContext::clearKeyEvent(KeyEvent event)
{
    glfwGetKey(window, event.key);
}

GLFWwindow* OpenGLContext::getWindow()
{
    return window;
}

double OpenGLContext::getDeltaTime()
{
    return deltaTime;
}

GLsizei OpenGLContext::getWidth()
{
    return width;
}

GLsizei OpenGLContext::getHeight()
{
    return height;
}

float OpenGLContext::getAspectRatio()
{
    return (float)width / height;
}

glm::dvec2 OpenGLContext::getMousePositionScreenSpace()
{
    return mousePosition * (1.0 / height);
}

glm::dvec2 OpenGLContext::getDeltaMousePosition()
{
    return deltaMousePosition;
}