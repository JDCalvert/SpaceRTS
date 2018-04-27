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
using namespace glm;

#include "OpenGLContext.h"
#include "Renderer.h"

#include "Camera.h"
#include "Controller.h"

#include "ResourceLoader.h"

#include "SimpleShader.h"
#include "LineShader.h"
#include "Surface.h"

int main()
{
    //Create an OpenGLContext. This will create a window for us with an OpenGL context
    OpenGLContext* glContext = OpenGLContext::initialiseNewContext();
    GLFWwindow* window = glContext->getWindow();

    //Use the basic renderer to draw to the screen and register it with our context
    Renderer* renderer = Renderer::createRenderer();
    glContext->addRenderer(renderer);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    Camera* camera = new Camera();
    Controller* controller = new Controller();    

    SimpleShader* simpleShader = new SimpleShader();
    simpleShader->initialise();

    LineShader* lineShader = new LineShader();
    lineShader->initialise();

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.objcomplete");
    surface->diffuseMap = ResourceLoader::loadDDS("Graphics/metalTexture.dds");
    
    //Projection matrix
    float minDrawDistance = 0.1f;
    float maxDrawDistance = 100.0f;
    float fieldOfView = 45.0f;
    int width = glContext->getWidth();
    int height = glContext->getHeight();
    glm::mat4 projectionMatrix = glm::perspective(fieldOfView, (float)width / height, minDrawDistance, maxDrawDistance);

    //Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    while (!controller->keyPressed(window, GLFW_KEY_ESCAPE)
        && glfwWindowShouldClose(window) == 0)
    {
        //Update the camera position
        controller->update(window);

        //Reset the frame so we're ready to draw
        renderer->initialiseFrame();

        glm::vec3 cameraPosition = controller->getCameraPosition();
        glm::vec3 cameraForward = controller->getCameraForward();
        glm::vec3 cameraUp = controller->getCameraUp();

        width = glContext->getWidth();
        height = glContext->getHeight();
        glm::mat4 projectionMatrix = glm::perspective(fieldOfView, (float)width / height, minDrawDistance, maxDrawDistance);
        glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
        glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        //Draw our cube
        //simpleShader->renderSurface(surface, modelViewProjectionMatrix);
        lineShader->renderSurface(surface, modelViewProjectionMatrix);

        //Now we've drawn everything to the renderer, draw to the window
        glContext->bindDefaultFrameBuffer();
        glContext->clearScreen();

        //We're done drawing things
        renderer->renderFrame();

        controller->cleanUpFrame();

        glContext->flip();
    }

    glfwTerminate();
}