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
#include "Controller.h"
#include "ResourceLoader.h"
#include "Renderer.h"

#include "Shader.h"
#include "SimpleShader.h"
#include "ScreenShader.h"

#include "Surface.h"

int main()
{
    OpenGLContext* glContext = OpenGLContext::initialiseNewContext();
    GLFWwindow* window = glContext->getWindow();

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);    
    glfwSetWindowSizeCallback(window, OpenGLContext::windowResized);

    Controller* controller = new Controller();
    ScreenShader* screenShader = new ScreenShader();

    Renderer* renderer = new Renderer();
    renderer->initialise();

    glContext->addRenderer(renderer);

    SimpleShader* simpleShader = new SimpleShader();
    simpleShader->initialise();

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.objcomplete");
    surface->diffuseMap = ResourceLoader::loadDDS("Graphics/metalTexture.dds");
    
    //Projection matrix
    float minDrawDistance = 0.1f;
    float maxDrawDistance = 500.0f;
    float fieldOfView = 45.0f;
    int width = glContext->getWidth();
    int height = glContext->getHeight();
    glm::mat4 projectionMatrix = glm::perspective(fieldOfView, (float)width / height, minDrawDistance, maxDrawDistance);

    //Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
        && glfwWindowShouldClose(window) == 0)
    {
        //Clear the screen to black
        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        simpleShader->renderSurface(surface, modelViewProjectionMatrix);

        //Now we've drawn everything to the renderer, draw the renderer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);

        //Clear the colour and depth buffers
        glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //We're done drawing things
        renderer->renderFrame();

        controller->cleanUpFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}