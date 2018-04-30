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
    Renderer* renderer = Renderer::createRenderer(glContext);
    glContext->addRenderer(renderer);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    Camera* camera = new Camera(glContext);
    Controller* controller = new Controller(camera, glContext);    

    SimpleShader* simpleShader = new SimpleShader(glContext);
    simpleShader->initialise();

    LineShader* lineShader = new LineShader(glContext);
    lineShader->initialise();

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.objcomplete");
    surface->diffuseMap = ResourceLoader::loadDDS("Graphics/metalTexture.dds");

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
        simpleShader->renderSurface(surface, modelViewProjectionMatrix);
        lineShader->renderSurface(surface, modelViewProjectionMatrix);

        //Now we've drawn everything to the renderer, draw to the window
        glContext->bindDefaultFrameBuffer();
        glContext->clearScreen();

        //We're done drawing things
        renderer->renderFrame();

        glContext->flip();
    }

    glfwTerminate();
}