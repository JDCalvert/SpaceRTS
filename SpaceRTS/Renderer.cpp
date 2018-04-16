#include "Renderer.h"
#include "ResourceLoader.h"
#include "WorldObject.h"
#include "Controller.h"
#include "Surface.h"
#include "World.h"
#include "UserInterfacePanel.h"
#include "OrderIndependentTransparencyShader.h"

#include <sstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

Renderer::Renderer(GLFWwindow* window, Controller* controller)
{
	this->window = window;
	this->controller = controller;
}

void Renderer::initialise()
{
	//Set up our rendering parameters
	minDrawDistance = 0.1f;
	maxDrawDistance = 500.0f;
	fieldOfView = 45.0f;
	antiAliasingFactor = 2;

	//Initialise the various shaders
	initialiseScreenShaderProgram();
	userInterfaceShader = new UserInterfaceShader(this);
	userInterfaceShader->initialise();

	textShader = new TextShader(this);
	textShader->initialise();

	userInterfaceBarShader = new UserInterfaceBarShader(this);
	userInterfaceBarShader->initialise();

	initialiseFramebuffer();

	orderIndependentTransparencyShader = new OrderIndependentTransparencyShader(this);
	orderIndependentTransparencyShader->initialise();

	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width/2, height/2);

	windowResized(width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::initialiseScreenShaderProgram()
{
	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "screenShader.vert"},
		{GL_FRAGMENT_SHADER, "screenShader.frag"}
	};
	screenShaderProgramId = ResourceLoader::loadShaders(shaders, 2);

	screenShaderVertexId	= glGetAttribLocation(screenShaderProgramId, "vertexPosition");
	screenShaderTextureId	= glGetUniformLocation(screenShaderProgramId, "renderedTexture");

	glGenVertexArrays(1, &screenVertexArrayId);
	glBindVertexArray(screenVertexArrayId);

	static const GLfloat screenCoords[] =
	{
		-1.0f, 1.0f, //Top left
		-1.0f,-1.0f, //Bottom left
		 1.0f, 1.0f, //Top right
		 1.0f,-1.0f  //Bottom right
	};

	glGenBuffers(1, &screenVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, screenVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), &screenCoords, GL_STATIC_DRAW);
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

	//Let the various shaders do their own resizing
	orderIndependentTransparencyShader->initialiseForScreenSize();

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

	//Let the various shaders do their own initialisation
	orderIndependentTransparencyShader->initialiseFrame();
}

void Renderer::renderFrame()
{
	//Calculate the frame rate and display it to the screen
	float frameRate = 1.0f / controller->deltaTime;
	int displayFrameRate = (int)(frameRate + 0.5f);
	std::ostringstream number;
	number << displayFrameRate;
	//std::string frameRateStr = number.str();
	
	GLboolean depthEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	std::string frameRateStr = "Hello Katie!!";
	textShader->renderText(&frameRateStr[0], 0, 0, 0.1f, screenRatio);

	if (depthEnabled) glEnable(GL_DEPTH_TEST);

	//Set the active framebuffer to be the actual screen and set our viewport to be the whole screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	//Clear the colour and depth buffers
	glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	glDisable(GL_BLEND);

	//Use the screen shader program
	glUseProgram(screenShaderProgramId);
	
	//Set the texture to be rendered to the screen
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glUniform1i(screenShaderTextureId, 0);

	//Use the buffer containing the whole-screen coordinates for the vertex positions
	glEnableVertexAttribArray(screenShaderVertexId);
	glBindBuffer(GL_ARRAY_BUFFER, screenVertexBufferId);
	glVertexAttribPointer(screenShaderVertexId, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(screenShaderVertexId);

	if (blendEnabled) glEnable(GL_BLEND);
}

void Renderer::renderWorld(World* world)
{
	//Update our list of lights from the world
	world->getLightSources(lightPositions);

	//Actually render the world
	world->render();

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glViewport(0, 0, getWidthAntialiasing(), getHeightAntialiasing());

	//Clear the colour and depth buffers
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Allow the order independent transparency shader
	orderIndependentTransparencyShader->renderFrame();
}

void Renderer::calculateViewProjectionMatrices()
{
	glm::vec3 cameraPosition = controller->getCameraPosition();
	glm::vec3 cameraForward = controller->getCameraForward();
	glm::vec3 cameraUp = controller->getCameraUp();

	projectionMatrix = glm::perspective(fieldOfView, (float)width/height, minDrawDistance, maxDrawDistance);
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
}

void Renderer::renderText(const char* text, float x, float y, float size)
{
	textShader->renderText(text, x, y, size, screenRatio);
}

void Renderer::renderBars(std::vector<glm::vec2> const &vertices, std::vector<glm::vec2> const &dimensions, std::vector<glm::vec4> const &colours)
{
	userInterfaceBarShader->render(vertices, dimensions, colours, screenRatio);
}

void Renderer::renderUserInterfacePanel(UserInterfacePanel* panel)
{
	glm::vec2 position = panel->position;
	float panelWidth = panel->width;
	float panelHeight = panel->height;
	float borderWidth = panel->borderwidth;

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> textureCoordinates;

	float left = position.x;
	float xMiddle = position.x + borderWidth;
	float right = position.x + panelWidth - borderWidth;
	float top = position.y;
	float yMiddle = position.y + borderWidth;
	float bottom = position.y + panelHeight - borderWidth;
	float middleWidth = panelWidth - 2*borderWidth;
	float middleHeight = panelHeight - 2*borderWidth;

	//Build the top left panel
	buildPanel(vertices, glm::vec2(left, top), borderWidth, borderWidth);
	buildPanel(textureCoordinates, glm::vec2(0, 0), 0.25f, 0.25f);

	//Build the top middle panel
	buildPanel(vertices, glm::vec2(xMiddle, top), middleWidth, borderWidth);
	buildPanel(textureCoordinates, glm::vec2(0.25f, 0.0f), 0.5f, 0.25f);

	//Build the top right panel
	buildPanel(vertices, glm::vec2(right, top), borderWidth, borderWidth);
	buildPanel(textureCoordinates, glm::vec2(0.75f, 0.0f), 0.25f, 0.25f);

	//Build the middle left panel
	buildPanel(vertices, glm::vec2(left, yMiddle), borderWidth, middleHeight);
	buildPanel(textureCoordinates, glm::vec2(0, 0.25f), 0.25f, 0.5f);

	//Build the main panel
	buildPanel(vertices, glm::vec2(xMiddle, yMiddle), middleWidth, middleHeight);
	buildPanel(textureCoordinates, glm::vec2(0.25f, 0.25f), 0.5f, 0.5f);

	//Build the middle right panel
	buildPanel(vertices, glm::vec2(right, yMiddle), borderWidth, middleHeight);
	buildPanel(textureCoordinates, glm::vec2(0.75f, 0.25f), 0.25f, 0.5f);

	//Build the bottom left panel
	buildPanel(vertices, glm::vec2(left, bottom), borderWidth, borderWidth);
	buildPanel(textureCoordinates, glm::vec2(0, 0.75f), 0.25f, 0.25f);

	//Build the bottom middle panel
	buildPanel(vertices, glm::vec2(xMiddle, bottom), middleWidth, borderWidth);
	buildPanel(textureCoordinates, glm::vec2(0.25f, 0.75f), 0.5f, 0.25f);

	//Build the bottom right panel
	buildPanel(vertices, glm::vec2(right, bottom), borderWidth, borderWidth);
	buildPanel(textureCoordinates, glm::vec2(0.75f, 0.75f), 0.25f, 0.25f);

	GLuint texture = panel->texture;

	userInterfaceShader->render(vertices, textureCoordinates, texture, screenRatio);
}

void Renderer::buildPanel(std::vector<glm::vec2> &vertices, glm::vec2 topLeft, float width, float height)
{
	glm::vec2 topRight		= topLeft + glm::vec2(width,	  0);
	glm::vec2 bottomLeft	= topLeft + glm::vec2(	  0, height);
	glm::vec2 bottomRight	= topLeft + glm::vec2(width, height);

	vertices.push_back(topLeft);
	vertices.push_back(bottomLeft);
	vertices.push_back(topRight);
	vertices.push_back(topRight);
	vertices.push_back(bottomLeft);
	vertices.push_back(bottomRight);
}

void Renderer::bindArrayBufferData(GLuint bufferId, int size, void* dataPointer)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, dataPointer, GL_STATIC_DRAW);
}

void Renderer::bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, dataPointer, GL_STATIC_DRAW);
}

void Renderer::enableVertexAttrib(GLuint attribId, GLuint bufferId, int attribSize)
{
	glEnableVertexAttribArray(attribId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glVertexAttribPointer(attribId, attribSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
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