#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "OpenGLContext.h"
#include "Renderer.h"

#include "UIRenderer.h"
#include "Camera.h"
#include "Controller.h"

#include "ResourceLoader.h"

#include "SimpleShader.h"
#include "LineShader.h"
#include "PointShader.h"
#include "UIShader.h"

#include "Surface.h"

#include "UIPanel.h"
#include "UIButton.h"

int main()
{
    //Create an OpenGLContext. This will create a window for us with an OpenGL context
    OpenGLContext* glContext = OpenGLContext::initialiseNewContext();
    GLFWwindow* window = glContext->getWindow();

    //Use the basic renderer to draw to the screen and register it with our context
    Renderer* renderer = Renderer::createRenderer();
    glContext->addRenderer(renderer);

    UIRenderer* uiRenderer = UIRenderer::createRenderer();
    glContext->addRenderer(uiRenderer);

    Camera* camera = new Camera();
    Controller* controller = new Controller(camera);    

    SimpleShader* simpleShader = new SimpleShader();
    simpleShader->initialise();

    LineShader* lineShader = new LineShader();
    lineShader->initialise();

    PointShader* pointShader = new PointShader();
    pointShader->initialise();

    UIShader* uiShader = new UIShader();
    uiShader->initialise();

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.objcomplete");
    surface->diffuseMap = ResourceLoader::loadDDS("Graphics/metalTexture.dds");

    UIPanel* uiPanel = new UIPanel();
    uiPanel->setPositionAndSize(glm::vec2(0.01f, 0.01f), glm::vec2(0.34f, 0.12f));

    UIButton* surfaceButton = new UIButton();
    surfaceButton->setPositionAndSize(glm::vec2(0.01f, 0.01f), glm::vec2(0.1f, 0.1f));
    uiPanel->addComponent(surfaceButton);

    UIButton* lineButton = new UIButton();
    lineButton->setPositionAndSize(glm::vec2(0.12f, 0.01f), glm::vec2(0.1f, 0.1f));
    uiPanel->addComponent(lineButton);

    UIButton* pointButton = new UIButton();
    pointButton->setPositionAndSize(glm::vec2(0.23f, 0.01f), glm::vec2(0.1f, 0.1f));
    uiPanel->addComponent(pointButton);

    //Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    while (!glContext->keyPressed(GLFW_KEY_ESCAPE)
        && glfwWindowShouldClose(window) == 0)
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
        if (surfaceButton->toggle) simpleShader->renderSurface(surface, modelViewProjectionMatrix);
        if (lineButton->toggle) lineShader->renderSurface(surface, modelViewProjectionMatrix);
        if (pointButton->toggle) pointShader->renderSurface(surface, modelViewProjectionMatrix);

        //Draw the UI
        uiRenderer->initialiseFrame();
        uiShader->renderUiComponent(uiPanel);

        //Now we've drawn everything to the renderer, draw to the window
        glContext->bindDefaultFrameBuffer();
        glContext->clearScreen();

        //Draw the renderers onto the context
        renderer->renderFrame();
        uiRenderer->renderFrame();

        MouseEvent* mouseEvent = OpenGLContext::currentContext()->nextMouseEvent();
        if (mouseEvent != nullptr)
        {
            if (mouseEvent->action == GLFW_PRESS)
            {
                uiPanel->checkAndProcessMouseEvent(mouseEvent);
            }
            delete mouseEvent;
        }

        glContext->flip();
    }

    glfwTerminate();
}