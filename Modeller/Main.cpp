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
#include <TextureRenderer.h>

#include <Font.h>

#include "Camera.h"
#include "Controller.h"

#include "Texture.h"

#include "SimpleShader.h"
#include "BlankShader.h"
#include "UIShader.h"

#include "Surface.h"

#include "UserInterfaceModeller.h"

int main()
{
    //Create an OpenGLContext. This will create a window for us with an OpenGL context
    OpenGLContext* glContext = OpenGLContext::initialiseContext();

    //Use the basic renderer to draw to the screen and register it with our context
    Renderer* renderer = Renderer::initialiseRenderer(new Renderer(), glContext);
    Renderer* uiRenderer = Renderer::initialiseRenderer(new Renderer(), glContext);
    
    renderer->setClearColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    Camera* camera = new Camera();
    Controller* controller = new Controller(camera);

    SimpleShader* simpleShader = Shader::loadShader(new SimpleShader(), "Simple");
    BlankShader* blankShader = Shader::loadShader(new BlankShader(), "Blank");

    UIShader* uiShader = Shader::loadShader(new UIShader(), "UI");

    Texture::loadDDS("../Resources/Graphics/blank.dds", "Blank");
    Texture::loadDDS("../Resources/Graphics/blankDark.dds", "BlankDark");
    Texture::loadDDS("../Resources/Graphics/blankNothing.dds", "BlankNothing");

    Font::loadFont("../Resources/Graphics/font.bff", "Default");
    Font::loadFont("../Resources/Graphics/calibri.bff", "Calibri");

    Font& calibri = Font::getFont("Calibri");

    Surface* surface = new Surface();

    UserInterfaceModeller* ui = UserInterface::initialise(new UserInterfaceModeller(surface));
    ui->build();
    ui->loadSurface("../Resources/Models/cube.mesh");
    
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

        surface->recalculateModelBoneMatrices();

        //Draw our cube
        glm::vec4 colour(1.0f, 1.0f, 1.0f, 0.5f);
        if (ui->renderOptions->renderSurface) simpleShader->renderSurface(surface, viewProjectionMatrix);
        if (ui->renderOptions->renderLines) blankShader->renderLines(surface, viewProjectionMatrix, colour);
        if (ui->renderOptions->renderPoints) blankShader->renderVertices(surface, viewProjectionMatrix, colour);
        if (ui->renderOptions->renderBones) blankShader->renderBones(surface, viewProjectionMatrix);

        std::vector<unsigned int> highlightVertices = ui->getHighlightVertexIndices();
        blankShader->renderVertices(surface, highlightVertices, viewProjectionMatrix, glm::vec4(1.0f));

        std::vector<unsigned int> highlightIndices = ui->getHighlightTriangleIndices();
        blankShader->renderTriangles(surface, viewProjectionMatrix, highlightIndices, glm::vec4(1.0f, 1.0f, 1.0f, 0.75f));

        ui->renderTextureCoordinates();

        //Draw the UI
        uiRenderer->initialiseFrame();
        ui->render();

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