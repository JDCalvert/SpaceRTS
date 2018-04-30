#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <Renderer.h>

class OpenGLContext;

class UIRenderer : public Renderer
{
public:
    static UIRenderer* createRenderer();

    void initialise();

protected:
    int getSuperSampleFactor();
};

#endif