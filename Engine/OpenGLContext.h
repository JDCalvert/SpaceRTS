#ifndef OPEN_GL_CONTEXT_H
#define OPEN_GL_CONTEXT_H

#include <map>

#include <gl\glew.h>

class OpenGLContext
{
private:
    std::map<GLenum, bool> glCapabilities;

    void setEnabled(GLenum glCapability, bool enable);
};

#endif