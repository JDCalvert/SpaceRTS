#include "OpenGLContext.h"
#include "Renderer.h"


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
    if (enabled)
    {
          glEnable(glCapability);
    }
    else
    {
        glDisable(glCapability);
    }
}

void OpenGLContext::addRenderer(Renderer* renderer)
{
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