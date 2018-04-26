#include "Renderer.h"
#include "ScreenShader.h"

void Renderer::initialise()
{
    initialiseFrameBuffer();
    initialiseScreenShader();
}

void Renderer::initialiseFrameBuffer()
{
    //Generate this renderer's framebuffer
    glGenFramebuffers(1, &frameBufferId);

    //Generate the texture used as the framebuffer's render buffer
    glGenTextures(1, &frameTexture);
    glBindTexture(GL_TEXTURE_2D, frameTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Generate a render buffer to store depth
    glGenRenderbuffers(1, &depthRenderbufferId);

    //Check we've successfully generated the framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Failed to generate framebuffer");
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::initialiseScreenShader()
{
    screenShader = new ScreenShader();
    screenShader->initialise();
}

void Renderer::resize(int width, int height)
{
    this->width = width * 2;
    this->height = height * 2;
    
    recreateFramebuffer();
}

void Renderer::recreateFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    //Recreate the texture with the new width and height
    glBindTexture(GL_TEXTURE_2D, frameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    //Reset the framebuffer's texture to our recreated one
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameTexture, 0);

    //Recreate the depth renderbuffer and reattach it to the framebuffer
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferId);
}

void Renderer::initialiseFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderFrame()
{
    screenShader->render(frameTexture);
}