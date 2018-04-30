#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>
#include <glfw/glfw3.h>

class ScreenShader;
class OpenGLContext;

class Renderer
{
public:
    static Renderer* createRenderer(OpenGLContext* glContext);

    Renderer(OpenGLContext* glContext);

    void initialise();
    void initialiseFrame();
    void renderFrame();

    void resize(int width, int height);
    void recreateFramebuffer();

protected:
    
    OpenGLContext* glContext;
    ScreenShader* screenShader;

    GLuint frameBufferId;
    GLuint depthRenderbufferId;
    GLuint frameTexture;

    int width;
    int height;

    int getSuperSampleFactor();

    void initialiseFrameBuffer();
    void initialiseScreenShader();
};

#endif