#ifndef USER_INTERFACE_PANEL_H
#define USER_INTERFACE_PANEL_H

#include <glm\glm.hpp>

#include "Renderer.h"

class UserInterface;

class UserInterfacePanel
{
	public:
	UserInterfacePanel(UserInterface* userInterface);
	~UserInterfacePanel();

	UserInterface* userInterface;

	glm::vec2 position;
	float width;
	float height;
	float borderwidth;

	void render();

	GLuint texture;
};

#endif