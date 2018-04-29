#include "Controller.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "OpenGLContext.h"

Controller::Controller(Camera* camera, OpenGLContext* glContext)
{
    this->camera = camera;
    this->glContext = glContext;
}

void Controller::calculateCameraPosition()
{
    glm::ivec2 mousePos = 
	double xpos, ypos;
	glContext->getCursorPosition(&xpos, &ypos);

	if (mouseButtonPressed(window, GLFW_MOUSE_BUTTON_MIDDLE))
	{
		horizontalAngle -= mouseSpeed * deltaTime * float(previousMouseX - xpos);
		verticalAngle += mouseSpeed * deltaTime * float(previousMouseY - ypos);

		ensureAnglesWithinRange();
	}

    /*int count;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    horizontalAngle += deltaTime * axes[2];
    verticalAngle += deltaTime * axes[3];*/

	//The direction the camera is facing
	float forwardX = sin(horizontalAngle) * cos(verticalAngle);
	float forwardY = cos(horizontalAngle) * cos(verticalAngle);
	float forwardZ = sin(verticalAngle);
	cameraForward = glm::vec3(forwardX, forwardY, forwardZ);

	//The direction directly right to the camera (this is always horizontal and so isn't affected by the vertical angle
	float rightX = sin(horizontalAngle + PI / 2);
	float rightY = cos(horizontalAngle + PI / 2);
	float rightZ = 0.0f;
	cameraRight = glm::vec3(rightX, rightY, rightZ);

	//The direction directly up from the camera's perspective (this isn't directly up, it's up from the camera's perspective, which is the cross product of the forward and right directions
	cameraUp = glm::cross(cameraRight, cameraForward);

	//The direction the camera is facing on the horizontal axis. Used to move the camera
	glm::vec3 horizontalForward(sin(horizontalAngle), cos(horizontalAngle), 0.0f);

	if (keyPressed(window, GLFW_KEY_W)) cameraPosition += horizontalForward * deltaTime * speed;
	if (keyPressed(window, GLFW_KEY_S)) cameraPosition -= horizontalForward * deltaTime * speed;
	if (keyPressed(window, GLFW_KEY_D)) cameraPosition += cameraRight * deltaTime * speed;
	if (keyPressed(window, GLFW_KEY_A)) cameraPosition -= cameraRight * deltaTime * speed;
    if (keyPressed(window, GLFW_KEY_T)) cameraPosition += cameraUp * deltaTime * speed;
    if (keyPressed(window, GLFW_KEY_G)) cameraPosition -= cameraUp * deltaTime * speed;
	if (keyPressed(window, GLFW_KEY_Q)) horizontalAngle -= PI / 4 * deltaTime;
	if (keyPressed(window, GLFW_KEY_E)) horizontalAngle += PI / 4 * deltaTime;
    if (keyPressed(window, GLFW_KEY_R)) verticalAngle += PI / 4 * deltaTime;
    if (keyPressed(window, GLFW_KEY_F)) verticalAngle -= PI / 4 * deltaTime;
    
    

    /*cameraPosition += axes[1] * deltaTime * speed * horizontalForward;*/
    /*cameraPosition += axes[0] * deltaTime * speed * cameraRight;*/

	previousMouseX = xpos;
	previousMouseY = ypos;
}

void Controller::ensureAnglesWithinRange()
{
	if (horizontalAngle > PI)
	{
		horizontalAngle -= 2 * PI;
	}
	if (horizontalAngle < -PI)
	{
		horizontalAngle += 2 * PI;
	}

	if (verticalAngle > PI / 2)
	{
		verticalAngle = PI / 2;
	}
	if (verticalAngle < -PI / 2)
	{
		verticalAngle = -PI / 2;
	}
}

glm::vec3 Controller::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Controller::getCameraForward()
{
	return cameraForward;
}

glm::vec3 Controller::getCameraRight()
{
	return cameraRight;
}

glm::vec3 Controller::getCameraUp()
{
	return cameraUp;
}

glm::vec2 Controller::getMousePosition()
{
	return glm::vec2(previousMouseX, previousMouseY);
}