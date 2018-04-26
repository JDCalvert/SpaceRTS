#include "OpenGLContext.h"
#include "Renderer.h"

static std::unordered_map<GLFWwindow*, OpenGLContext*> contextsByWindow;

void OpenGLContext::windowResized(GLFWwindow* window, int width, int height)
{
    OpenGLContext* context = contextsByWindow[window];
    context->resize(width, height);
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

    //Configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No legacy stuff

    //Create a window with an OpenGL context
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

    OpenGLContext* context = new OpenGLContext();
    context->window = window;
    context->width = width;
    context->height = height;

    contextsByWindow[window] = context;

    return context;
}

void OpenGLContext::setEnabled(GLenum glCapability, GLboolean enabled)
{
    auto it = glCapabilities.find(glCapability);
    if (it == glCapabilities.end())
    {
        glCapabilities.insert(std::pair<GLenum, GLboolean>(glCapability, enabled));
        actuallySetEnabled(glCapability, enabled);
    }
    else if (it->second != enabled)
    {
        actuallySetEnabled(glCapability, enabled);
    }
}

void OpenGLContext::actuallySetEnabled(GLenum glCapability, GLboolean enabled)
{
    enabled ? glEnable(glCapability) : glDisable(glCapability);
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

GLFWwindow* OpenGLContext::getWindow()
{
    return window;
}

GLsizei OpenGLContext::getWidth()
{
    return width;
}

GLsizei OpenGLContext::getHeight()
{
    return height;
}