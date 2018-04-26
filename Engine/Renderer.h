#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>
#include <glfw/glfw3.h>

class ScreenShader;

class Renderer
{
public:

    void initialise();
    void initialiseFrame();
    void renderFrame();

    void resize(int width, int height);
    void recreateFramebuffer();

protected:
    
    ScreenShader* screenShader;

    GLuint frameBufferId;
    GLuint depthRenderbufferId;
    GLuint frameTexture;

    int width;
    int height;

    void initialiseFrameBuffer();
    void initialiseScreenShader();
};

#endif