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

#include "Controller.h"
#include "ResourceLoader.h"
#include "Renderer.h"

#include "Shader.h"
#include "SimpleShader.h"
#include "ScreenShader.h"

#include "Surface.h"

Renderer* renderer;

void windowResized(GLFWwindow* window, int width, int height)
{
    renderer->windowResized(width, height);
}

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialise GLFW");
        return -1;
    }

    //Configuration
    glfwWindowHint(GLFW_SAMPLES, 4); //Anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No legacy stuff

    int width = 800;
    int height = 600;

    //Create a window with an OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.");
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialise GLEW");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetWindowSizeCallback(window, windowResized);

    Controller* controller = new Controller();
    ScreenShader* screenShader = new ScreenShader();

    renderer = new Renderer(window, controller, screenShader);

    SimpleShader* simpleShader = new SimpleShader(renderer);
    renderer->addShader(simpleShader);

    renderer->initialise();

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.objcomplete");
    surface->diffuseMap = ResourceLoader::loadDDS("Graphics/metalTexture.dds");

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

        //Draw our cube
        simpleShader->renderSurface(surface, modelMatrix);

        //We're done drawing things
        renderer->renderFrame();

        controller->cleanUpFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}