#include "Renderer.h"
#include "ResourceLoader.h"
#include "Controller.h"
#include "Shader.h"
#include "ScreenShader.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

Renderer::Renderer(GLFWwindow* window, Controller* controller, ScreenShader* screenShader)
{
	this->window = window;
	this->controller = controller;
    this->screenShader = screenShader;
}

void Renderer::initialise()
{
	//Set up our rendering parameters
	minDrawDistance = 0.1f;
	maxDrawDistance = 500.0f;
	fieldOfView = 45.0f;
	antiAliasingFactor = 2;

    screenShader->initialise();

	//Initialise the shaders
	initialiseFramebuffer();

	for (unsigned int i=0; i<shaders.size(); i++)
	{
		Shader* shader = shaders[i];
		shader->initialise();
	}

	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width/2, height/2);

    windowResized(width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::initialiseFramebuffer()
{
	//Generate a framebuffer
	glGenFramebuffers(1, &frameBufferId);

	//Generate the texture that will be used as the framebuffer's render buffer
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Generate depth buffer
	glGenRenderbuffers(1, &depthRenderbufferId);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffers have failed");
	}
}

void Renderer::windowResized(int width, int height)
{
	//Update the width and height, and calculate the aspect ratio
	this->width = width;
	this->height = height;
	screenRatio = (float)width / height;

	//Tell the shaders that the screen has been resized
	for (int i = 0; i < shaders.size(); i++)
	{
		Shader* shader = shaders[i];
		shader->initialiseForScreenSize();
	}
	
	//Reset the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, getWidthAntialiasing(), getHeightAntialiasing(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getWidthAntialiasing(), getHeightAntialiasing());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferId);
}

void Renderer::initialiseFrame()
{
	//Set the active framebuffer to be our whole-screen framebuffer and set our viewport to be the whole framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glViewport(0, 0, getWidthAntialiasing(), getHeightAntialiasing());

	//Clear the colour and depth buffers
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Get the up to date view and projection matrices from the controller
	calculateViewProjectionMatrices();

	//Let our shaders do frame initialisation
	for (int i=0; i<shaders.size(); i++)
	{
		Shader* shader = shaders[i];
		shader->initialiseFrame();
	}
}

void Renderer::addShader(Shader* shader)
{
    shaders.push_back(shader);
}

void Renderer::renderFrame()
{
	//Set the active framebuffer to be the actual screen and set our viewport to be the whole screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	//Clear the colour and depth buffers
	glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLboolean blendEnabled = getAndSetGlCapability(GL_BLEND, false);

    screenShader->render(renderedTexture);

    setGlCapability(GL_BLEND, blendEnabled);
}

void Renderer::calculateViewProjectionMatrices()
{
	glm::vec3 cameraPosition = controller->getCameraPosition();
	glm::vec3 cameraForward = controller->getCameraForward();
	glm::vec3 cameraUp = controller->getCameraUp();

	projectionMatrix = glm::perspective(fieldOfView, (float)width/height, minDrawDistance, maxDrawDistance);
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
}

glm::mat4 Renderer::getViewProjectionMatrix()
{
	return projectionMatrix * viewMatrix;
}

glm::vec3 Renderer::getCameraPosition()
{
	return controller->getCameraPosition();
}

int Renderer::getWidthAntialiasing()
{
	return antiAliasingFactor * width;
}

int Renderer::getHeightAntialiasing()
{
	return antiAliasingFactor * height;
}

int Renderer::getPixelCountAntialiasing()
{
	return antiAliasingFactor * antiAliasingFactor * width * height;
}

GLboolean Renderer::getAndSetGlCapability(GLenum cap, GLboolean enable)
{
	GLboolean isEnabled = glIsEnabled(cap);
	setGlCapability(cap, enable);
	return isEnabled;
}

void Renderer::setGlCapability(GLenum cap, GLboolean enable)
{
	if (enable)
	{
		glEnable(cap);
	}
	else
	{
		glDisable(cap);
	}
}