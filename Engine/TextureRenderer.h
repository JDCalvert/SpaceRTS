#ifndef TEXTURE_RENDERER_H
#define TEXTURE_RENDERER_H

#include "Renderer.h"

class TextureRenderer : public Renderer
{
public:
    TextureRenderer(float superSampleFactor);

    void resize(int width, int height) override;
};

#endif