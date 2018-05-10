#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "OpenGLContext.h"
#include "Renderer.h"

#include <Font.h>

#include "UIRenderer.h"
#include "Camera.h"
#include "Controller.h"

#include "Texture.h"

#include "SimpleShader.h"
#include "LineShader.h"
#include "PointShader.h"
#include "UIShader.h"

#include "Surface.h"

#include "UserInterface.h"
#include "UIVertexInformation.h"
#include "UIRenderOptions.h"

int main()
{
    //Create an OpenGLContext. This will create a window for us with an OpenGL context
    OpenGLContext* glContext = OpenGLContext::initialiseNewContext();

    //Use the basic renderer to draw to the screen and register it with our context
    Renderer* renderer = Renderer::createRenderer();
    glContext->addRenderer(renderer);

    UIRenderer* uiRenderer = UIRenderer::createRenderer();
    glContext->addRenderer(uiRenderer);

    Camera* camera = new Camera();
    Controller* controller = new Controller(camera);

    SimpleShader* simpleShader = Shader::loadShader(new SimpleShader(), "Simple");
    LineShader* lineShader = Shader::loadShader(new LineShader(), "Line");
    PointShader* pointShader = Shader::loadShader(new PointShader(), "Point");

    UIShader* uiShader = Shader::loadShader(new UIShader(), "UI");

    Texture::loadDDS("Graphics/metalTexture.dds", "Metal");
    Texture::loadDDS("Graphics/blank.dds", "Blank");
    Texture::loadDDS("Graphics/blankDark.dds", "BlankDark");

    Font::loadFont("Graphics/font.bff", "Default");
    Font::loadFont("Graphics/calibriLarge.bff", "Calibri");

    Font& calibri = Font::getFont("Calibri");

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.mesh");
    surface->diffuseMap = Texture::getTexture("Metal");

    UserInterface* ui = new UserInterface();

    UIRenderOptions* renderOptions = new UIRenderOptions();
    renderOptions->build();
    ui->addComponent(renderOptions);

    UIVertexInformation* vertexPanel = new UIVertexInformation();
    vertexPanel->build(surface);
    ui->addComponent(vertexPanel);

    //Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    while (!glContext->shouldClose())
    {
        glContext->initialiseFrame();

        //Update the camera position
        controller->update();

        //Reset the frame so we're ready to draw
        renderer->initialiseFrame();

        glm::mat4 projectionMatrix = camera->getProjectionMatrix();
        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        //Draw our cube
        if (renderOptions->renderSurface) simpleShader->renderSurface(surface, modelViewProjectionMatrix);
        if (renderOptions->renderLines) lineShader->renderSurface(surface, modelViewProjectionMatrix);
        if (renderOptions->renderPoints) pointShader->renderSurface(surface, modelViewProjectionMatrix);

        //Draw the UI
        uiRenderer->initialiseFrame();
        ui->render();

        //Now we've drawn everything to the renderer, draw to the window
        glContext->bindDefaultFrameBuffer();
        glContext->clearScreen();

        //Draw the renderers onto the context
        renderer->renderFrame();
        uiRenderer->renderFrame();

        ui->handleEvents();

        glContext->flip();
    }

    glfwTerminate();
}