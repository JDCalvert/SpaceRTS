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

#include <OpenGLContext.h>
#include <Renderer.h>

#include <Font.h>

#include "UIRenderer.h"
#include "Camera.h"
#include "Controller.h"

#include "Texture.h"

#include "SimpleShader.h"
#include "BlankShader.h"
#include "UIShader.h"

#include "Surface.h"

#include "UserInterfaceModeller.h"
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
    BlankShader* blankShader = Shader::loadShader(new BlankShader(), "Blank");

    UIShader* uiShader = Shader::loadShader(new UIShader(), "UI");

    Texture::loadDDS("Graphics/metalTexture.dds", "Metal");
    Texture::loadDDS("Graphics/blank.dds", "Blank");
    Texture::loadDDS("Graphics/blankDark.dds", "BlankDark");
    Texture::loadDDS("Graphics/blankNothing.dds", "BlankNothing");

    Font::loadFont("Graphics/font.bff", "Default");
    Font::loadFont("Graphics/calibri.bff", "Calibri");

    Font& calibri = Font::getFont("Calibri");

    Surface* surface = new Surface();
    surface->loadFromFile("Models/cube.mesh");
    surface->diffuseMap = Texture::getTexture("Metal");

    /*std::vector<glm::vec3>& vertices = surface->getVertices();
    std::vector<glm::vec3>& normals = surface->getNormals();
    for (unsigned int i=0; i<vertices.size(); i++)
    {
        vertices[i] += normals[i];
    }*/

    UserInterfaceModeller* ui = UserInterface::initialise(new UserInterfaceModeller());
    ui->build(surface);
    
    while (!glContext->shouldClose())
    {
        ui->handleEvents();
        
        glContext->initialiseFrame();

        //Update the camera position
        controller->update();

        //Reset the frame so we're ready to draw
        renderer->initialiseFrame();

        glm::mat4 projectionMatrix = camera->getProjectionMatrix();
        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

        //Draw our cube
        if (ui->renderOptions->renderSurface) simpleShader->renderSurface(surface, viewProjectionMatrix);
        if (ui->renderOptions->renderLines) blankShader->renderLines(surface, viewProjectionMatrix);
        if (ui->renderOptions->renderPoints) blankShader->renderVertices(surface, viewProjectionMatrix);

        //Draw the UI
        uiRenderer->initialiseFrame();
        ui->render();

        std::vector<glm::vec3> highlightVertices = ui->getHighlightVertices();
        blankShader->renderVertices(highlightVertices, viewProjectionMatrix);

        std::vector<unsigned int> highlightIndices = ui->getHighlightIndices();
        blankShader->renderTriangles(surface, viewProjectionMatrix, highlightIndices);

        //Now we've drawn everything to the renderer, draw to the window
        glContext->bindDefaultFrameBuffer();
        glContext->clearScreen();

        //Draw the renderers onto the context
        renderer->renderFrame();
        uiRenderer->renderFrame();

        glContext->flip();
    }

    glfwTerminate();
}