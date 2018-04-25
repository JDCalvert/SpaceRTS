#include "OpenGLContext.h"

void OpenGLContext::setEnabled(GLenum glCapability, bool enable)
{
    auto it = glCapabilities.find(glCapability);
    if (it == glCapabilities.end())
    {
        glCapabilities.insert(std::pair<GLenum, bool>(glCapability, enable));


    }
    else
    {

    }

    bool enabled = glCapabilities[glCapability];
    if ()
}