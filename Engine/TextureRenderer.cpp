#include "TextureRenderer.h"

TextureRenderer::TextureRenderer(float superSampleFactor) : Renderer(superSampleFactor)
{
}

void TextureRenderer::resize(int width, int height)
{
    this->width = height * superSampleFactor;
    this->height = height * superSampleFactor;

    recreateFramebuffer();
}