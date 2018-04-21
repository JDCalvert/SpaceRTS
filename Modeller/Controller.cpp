#include "Controller.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const double PI = 4 * atan(1.0);

Controller::Controller()
{
	cameraPosition = glm::vec3(3.0f, 3.0f, 3.0f);

	horizontalAngle = -3 * PI / 4;
	verticalAngle = -PI / 4;
	fieldOfView = 45.0f;

	speed = 3.0f;
	mouseSpeed = 0.05f;
	turnSpeed = 2.0f;

	time = glfwGetTime();

	leftMouseButtonPressed = false;
	leftMouseButtonDown = false;

	middleMouseButtonPressed = false;
	middleMouseButtonDown = false;

	rightMouseButtonPressed = false;
	rightMouseButtonDown = false;
}

bool Controller::keyPressed(GLFWwindow* window, int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Controller::mouseButtonPressed(GLFWwindow* window, int key)
{
	return glfwGetMouseButton(window, key) == GLFW_PRESS;
}

void Controller::mouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			leftMouseButtonPressed = true;
			leftMouseButtonDown = true;
		}
		else
		{
			leftMouseButtonDown = false;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			rightMouseButtonPressed = true;
			rightMouseButtonDown = true;
		}
		else
		{
			rightMouseButtonDown = false;
		}
	}
}

void Controller::scrollCallback(double xOffset, double yOffset)
{
	cameraPosition += glm::vec3(0.0, -yOffset * 0.1, 0.0);
}

void Controller::update(GLFWwindow* window)
{
	calculateCameraPosition(window);
}

void Controller::cleanUpFrame()
{
	leftMouseButtonPressed = false;
	middleMouseButtonPressed = false;
	rightMouseButtonPressed = false;
}

void Controller::calculateCameraPosition(GLFWwindow* window)
{
	double previousTime = time;
	time = glfwGetTime();
	deltaTime = time - previousTime;

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (mouseButtonPressed(window, GLFW_MOUSE_BUTTON_MIDDLE))
	{
		horizontalAngle -= mouseSpeed * deltaTime * float(previousMouseX - xpos);
		verticalAngle += mouseSpeed * deltaTime * float(previousMouseY - ypos);

		ensureAnglesWithinRange();
	}

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

	if (keyPressed(window, GLFW_KEY_W))
	{
		cameraPosition += horizontalForward * deltaTime * speed;
	}
	if (keyPressed(window, GLFW_KEY_S))
	{
		cameraPosition -= horizontalForward * deltaTime * speed;
	}
	if (keyPressed(window, GLFW_KEY_D))
	{
		cameraPosition += cameraRight * deltaTime * speed;
	}
	if (keyPressed(window, GLFW_KEY_A))
	{
		cameraPosition -= cameraRight * deltaTime * speed;
	}
	if (keyPressed(window, GLFW_KEY_Q))
	{
		horizontalAngle -= PI / 4 * deltaTime;
	}
	if (keyPressed(window, GLFW_KEY_E))
	{
		horizontalAngle += PI / 4 * deltaTime;
	}

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