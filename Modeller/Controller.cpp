#include "Controller.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "OpenGLContext.h"

Controller::Controller(Camera* camera)
{
    this->camera = camera;
}

void Controller::update()
{
    OpenGLContext* glContext = OpenGLContext::currentContext();

    if (glContext->mouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
	{
        glm::dvec2 deltaMousePosition = glContext->getDeltaMousePosition();
        camera->rotate(deltaMousePosition);
	}

    if (glContext->keyPressed(GLFW_KEY_Q)) camera->lookLeft();
    if (glContext->keyPressed(GLFW_KEY_E)) camera->lookRight();
    if (glContext->keyPressed(GLFW_KEY_R)) camera->lookUp();
    if (glContext->keyPressed(GLFW_KEY_F)) camera->lookDown();

    camera->recalculateDirections();

    if (glContext->keyPressed(GLFW_KEY_W)) camera->moveForward();
    if (glContext->keyPressed(GLFW_KEY_S)) camera->moveBackward();
    if (glContext->keyPressed(GLFW_KEY_D)) camera->moveRight();
    if (glContext->keyPressed(GLFW_KEY_A)) camera->moveLeft();
    if (glContext->keyPressed(GLFW_KEY_T)) camera->moveUp();
    if (glContext->keyPressed(GLFW_KEY_G)) camera->moveDown(); 
}

glm::vec2 Controller::getMousePosition()
{
	return glm::vec2(previousMouseX, previousMouseY);
}