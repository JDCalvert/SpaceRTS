#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm\glm.hpp>

#include "WorldObject.h"
#include "Surface.h"
#include "ResourceLoader.h"
#include "Controller.h"
#include "Renderer.h"
#include "World.h"
#include "UserInterface.h"
#include "Geometry.h"

GLFWwindow* window;
Renderer* renderer;
Controller* controller;

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
	renderer->windowResized(width, height);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	controller->mouseButtonCallback(button, action, mods);
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	controller->scrollCallback(xOffset, yOffset);
}

/**
 * Initialise the main window
 */
void init()
{
	if (!glfwInit())
	{
		printf("GLFW failed to init");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1280, 960, "Space RTS", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	//glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("GLEW failed to init");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);
}

void main()
{
	init();

	controller = new Controller();
	renderer = new Renderer(window, controller);
	World* world = new World(renderer);
	UserInterface* userInterface = new UserInterface(renderer, world);

	renderer->initialise();

	int previousNumberOfSeconds = 0;
	int numberOfFramesOnThisSecond = 1;
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		double time = glfwGetTime();
		int numberOfSeconds = (int)time;
		if (numberOfSeconds == previousNumberOfSeconds)
		{
			numberOfFramesOnThisSecond++;
		}
		else
		{
			std::cout << numberOfFramesOnThisSecond << std::endl;
			previousNumberOfSeconds = numberOfSeconds;
			numberOfFramesOnThisSecond = 1;
		}

		controller->update(window);
		world->update();
		renderer->initialiseFrame();
		renderer->renderWorld(world);
		renderer->renderFrame();
		userInterface->update();
		userInterface->render();
		controller->cleanUpFrame();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

