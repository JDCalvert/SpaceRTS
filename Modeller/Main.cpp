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
#include "Surface.h"

Renderer* renderer;

Surface* generateTestSurface();

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

    renderer = new Renderer(window, controller);

    SimpleShader* simpleShader = new SimpleShader(renderer);
    renderer->addShader(simpleShader);

    renderer->initialise();

    Surface* cube = generateTestSurface();

    //Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    static const GLfloat vertexBufferData[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
        && glfwWindowShouldClose(window) == 0)
    {
        //Update the camera position
        controller->update(window);

        //Reset the frame so we're ready to draw
        renderer->initialiseFrame();

        //Draw our cube
        simpleShader->renderSurface(cube, modelMatrix);

        //We're done drawing things
        renderer->renderFrame();

        controller->cleanUpFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Surface* generateTestSurface()
{
    //Set up a surface so we can render it
    static GLfloat vertexPositionData[] =
    {
        //Front (y = 1)
        1.0f,  1.0f, -1.0f, //Bottom left
        -1.0f,  1.0f, -1.0f, //Bottom right
        -1.0f,  1.0f,  1.0f, //Top right
        1.0f,  1.0f,  1.0f, //Top left

                            //Left (x = -1)
                            -1.0f,  1.0f, -1.0f, //Bottom left
                            -1.0f, -1.0f, -1.0f, //Bottom right
                            -1.0f, -1.0f,  1.0f, //Top right
                            -1.0f,  1.0f,  1.0f, //Top left

                                                 //Back (y = -1)
                                                 -1.0f, -1.0f, -1.0f, //Bottom left
                                                 1.0f, -1.0f, -1.0f, //Bottom right
                                                 1.0f, -1.0f,  1.0f, //Top right
                                                 -1.0f, -1.0f,  1.0f, //Top left

                                                                      //Right (x = 1)
                                                                      1.0f, -1.0f, -1.0f, //Bottom left
                                                                      1.0f,  1.0f, -1.0f, //Bottom right
                                                                      1.0f,  1.0f,  1.0f, //Top right
                                                                      1.0f, -1.0f,  1.0f  //Top left
    };

    static GLfloat vertexTextureCoordinateData[] =
    {
        0.0f, 0.0f, //Bottom left
        1.0f, 0.0f, //Bottom right
        1.0f, 1.0f, //Top right
        0.0f, 1.0f, //Top left

        0.0f, 0.0f, //Bottom left
        1.0f, 0.0f, //Bottom right
        1.0f, 1.0f, //Top right
        0.0f, 1.0f, //Top left

        0.0f, 0.0f, //Bottom left
        1.0f, 0.0f, //Bottom right
        1.0f, 1.0f, //Top right
        0.0f, 1.0f, //Top left

        0.0f, 0.0f, //Bottom left
        1.0f, 0.0f, //Bottom right
        1.0f, 1.0f, //Top right
        0.0f, 1.0f  //Top left
    };

    static unsigned int indicesBuffer[] =
    {
        0,  1,  2,
        0,  2,  3,

        4,  5,  6,
        4,  6,  7,

        8,  9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15
    };

    std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>();
    std::vector<glm::vec2>* textureCoordinates = new std::vector<glm::vec2>();
    std::vector<unsigned int>* indices = new std::vector<unsigned int>();

    int numVertices = sizeof(vertexPositionData) / sizeof(GLfloat) / 3;
    for (int i = 0; i<numVertices; i++)
    {
        glm::vec3 vertexPosition
        (
            vertexPositionData[i * 3],
            vertexPositionData[i * 3 + 1],
            vertexPositionData[i * 3 + 2]
        );

        vertices->push_back(vertexPosition);

        glm::vec2 textureCoordinate
        (
            vertexTextureCoordinateData[i * 2],
            vertexTextureCoordinateData[i * 2 + 1]
        );

        textureCoordinates->push_back(textureCoordinate);
    }

    int numIndices = sizeof(indicesBuffer) / sizeof(unsigned int);
    for (int i = 0; i<numIndices; i++)
    {
        indices->push_back(indicesBuffer[i]);
    }

    //Use a texture
    GLuint diffuseTextureId = ResourceLoader::loadDDS("Graphics/metalTexture.dds");

    Surface* surface = new Surface();
    surface->setVertices(vertices);
    surface->setTextureCoordinates(textureCoordinates);
    surface->setIndices(indices);
    surface->diffuseMap = diffuseTextureId;

    surface->calculateSizesAndLength();

    return surface;
}