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

OpenGLContext* OpenGLContext::currentContext()
{
    return currentGlContext;
}

OpenGLContext* OpenGLContext::initialiseNewContext()
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

    OpenGLContext* context = new OpenGLContext();
    context->window = window;
    context->width = width;
    context->height = height;

    setCurrentContext(context);

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

bool OpenGLContext::mouseButtonPressed(int key)
{
    return glfwGetMouseButton(window, key) == GLFW_PRESS;
}

bool OpenGLContext::keyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
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

glm::dvec2 OpenGLContext::getDeltaMousePosition()
{
    return deltaMousePosition;
}