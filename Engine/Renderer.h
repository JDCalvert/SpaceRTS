#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class ScreenShader;
class OpenGLContext;

class Renderer
{
public:
    template <class T>
    static T* initialiseRenderer(T* renderer, OpenGLContext* glContext)
    {
        renderer->initialise();
        glContext->addRenderer(renderer);
        return renderer;
    }

    Renderer();
    Renderer(float superSampleFactor);

    void initialise();

    void bindRenderer();
    void initialiseFrame();
    void renderFrame();

    virtual void resize(int width, int height);
    void recreateFramebuffer();

    void setClearColour(glm::vec4 colour);

    GLuint getTexture();

protected:
    ScreenShader* screenShader;

    GLuint frameBufferId;
    GLuint depthRenderbufferId;
    GLuint frameTexture;

    int width;
    int height;
    float superSampleFactor;

    glm::vec4 clearColour;

    void initialiseFrameBuffer();
    void initialiseScreenShader();
};

#endif