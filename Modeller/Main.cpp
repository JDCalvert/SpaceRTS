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
#include "UILabel.h"

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

    GLuint metalTexture = ResourceLoader::loadDDS("Graphics/metalTexture.dds");

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.mesh");
    surface->diffuseMap = metalTexture;

    UIPanel* uiPanel = new UIPanel();
    uiPanel->setPositionAndSize(glm::vec2(0.01f, 0.01f), glm::vec2(0.34f, 0.12f));
    uiPanel->surface->diffuseMap = metalTexture;

    UIButton* surfaceButton = new UIButton();
    surfaceButton->setPositionAndSize(glm::vec2(0.01f, 0.01f), glm::vec2(0.1f, 0.1f));
    surfaceButton->surface->diffuseMap = metalTexture;
    uiPanel->addComponent(surfaceButton);
    
    UIButton* lineButton = new UIButton();
    lineButton->setPositionAndSize(glm::vec2(0.12f, 0.01f), glm::vec2(0.1f, 0.1f));
    lineButton->surface->diffuseMap = metalTexture;
    uiPanel->addComponent(lineButton);

    UIButton* pointButton = new UIButton();
    pointButton->setPositionAndSize(glm::vec2(0.23f, 0.01f), glm::vec2(0.1f, 0.1f));
    pointButton->surface->diffuseMap = metalTexture;
    uiPanel->addComponent(pointButton);

    Font* font = ResourceLoader::loadBFF("Graphics/font.bff");
    font->textureId = ResourceLoader::loadDDS("Graphics/font.dds");

    UIPanel* vertexPanel = new UIPanel();

    std::vector<glm::vec3>& vertices = surface->getVertices();
    std::vector<glm::vec2>& textureCoordinates = surface->getTextureCoordinates();
    float yPos = 0.01f;
    float textSize = 0.025f;
    float labelWidth = 0.10f;

    float xLabelPos = 0.01f;
    float yLabelPos = xLabelPos + labelWidth + 0.01f;
    float zLabelPos = yLabelPos + labelWidth + 0.01f;

    UILabel* xPosLabel = new UILabel();
    xPosLabel->setPositionAndSize(glm::vec2(xLabelPos, yPos), glm::vec2(labelWidth, 0.01f));
    xPosLabel->setText("x", textSize, *font, CENTRE);
    vertexPanel->addComponent(xPosLabel);

    UILabel* yPosLabel = new UILabel();
    yPosLabel->setPositionAndSize(glm::vec2(yLabelPos, yPos), glm::vec2(labelWidth, 0.01f));
    yPosLabel->setText("y", textSize, *font, CENTRE);
    vertexPanel->addComponent(yPosLabel);

    UILabel* zPosLabel = new UILabel();
    zPosLabel->setPositionAndSize(glm::vec2(zLabelPos, yPos), glm::vec2(labelWidth, 0.01f));
    zPosLabel->setText("y", textSize, *font, CENTRE);
    vertexPanel->addComponent(zPosLabel);

    for (unsigned int i=0; i<vertices.size(); i++)
    {
        glm::vec3 vertexPosition = vertices[i];
        glm::vec2 textureCoordinate = textureCoordinates[i];

        yPos += textSize;

        std::stringstream ssx;
        ssx << std::fixed << std::setprecision(3) << vertexPosition.x;
        std::string vertexX = ssx.str();
        
        UILabel* xPosValueLabel = new UILabel();
        xPosValueLabel->setPositionAndSize(glm::vec2(xLabelPos, yPos), glm::vec2(labelWidth, textSize));
        xPosValueLabel->setText(vertexX, textSize, *font, RIGHT);
        vertexPanel->addComponent(xPosValueLabel);

        std::stringstream ssy;
        ssy << std::fixed << std::setprecision(3) << vertexPosition.y;
        std::string vertexY = ssy.str();

        UILabel* yPosValueLabel = new UILabel();
        yPosValueLabel->setPositionAndSize(glm::vec2(yLabelPos, yPos), glm::vec2(labelWidth, textSize));
        yPosValueLabel->setText(vertexY, textSize, *font, RIGHT);
        vertexPanel->addComponent(yPosValueLabel);

        std::stringstream ssz;
        ssz << std::fixed << std::setprecision(3) << vertexPosition.z;
        std::string vertexZ = ssz.str();

        UILabel* zPosValueLabel = new UILabel();
        zPosValueLabel->setPositionAndSize(glm::vec2(zLabelPos, yPos), glm::vec2(labelWidth, textSize));
        zPosValueLabel->setText(vertexZ, textSize, *font, RIGHT);
        vertexPanel->addComponent(zPosValueLabel);
    }

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

        vertexPanel->setPositionAndSize(glm::vec2(OpenGLContext::currentContext()->getAspectRatio() - 0.50f, 0.01f), glm::vec2(0.49f, yPos));
        uiShader->renderUiComponent(vertexPanel);

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