#include "UIRenderer.h"

UIRenderer* UIRenderer::createRenderer()
{
    UIRenderer* renderer = new UIRenderer();
    renderer->initialise();

    return renderer;
}

void UIRenderer::initialise()
{
    Renderer::initialise();
}

int UIRenderer::getSuperSampleFactor()
{
    return 1;
}