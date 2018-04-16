#include "UserInterface.h"
#include "UserInterfacePanel.h"
#include "ObjectPanel.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "WorldObject.h"
#include "Geometry.h"
#include "World.h"
#include "Controller.h"

#include <sstream>

UserInterface::UserInterface(Renderer* renderer, World* world)
{
	this->renderer = renderer;
	this->world = world;

	hoverObject = 0;

	GLuint texture = ResourceLoader::loadDDS("Graphics/unitPanel.dds");

	objectPanel = new ObjectPanel(this);
	objectPanel->position = glm::vec2(0.0f, 0.6f);
	objectPanel->width = 0.3f;
	objectPanel->height = 0.4f;
	objectPanel->borderwidth = 0.01f;
	objectPanel->texture = texture;
}

UserInterface::~UserInterface()
{
}

void UserInterface::update()
{
	Controller* controller = renderer->controller;
	glm::vec2 mousePosition = controller->getMousePosition();

	//Convert the mouse coordinates into screen space
	float x = (2 * mousePosition.x / renderer->width) - 1;
	float y = (2 * -mousePosition.y / renderer->height) + 1;

	//Convert these into world space
	glm::vec4 mousePosition1 = glm::vec4(x, y, -1.0f, 1.0f);
	//glm::vec4 mousePosition2 = glm::vec4(x, y,  1.0f, 1.0f);
	glm::vec4 mousePosition2 = glm::vec4(x, y,  0.99f, 1.0f);

	glm::mat4 viewProjectionMatrix = renderer->getViewProjectionMatrix();
	glm::mat4 inverseViewProjectionMatrix = glm::inverse(viewProjectionMatrix);

	glm::vec4 mousePositionWorld1 = inverseViewProjectionMatrix * mousePosition1;
	glm::vec4 mousePositionWorld2 = inverseViewProjectionMatrix * mousePosition2;

	mousePositionWorld1 /= mousePositionWorld1.w;
	mousePositionWorld2 /= mousePositionWorld2.w;

	hoverObject = world->getHoverObject(mousePositionWorld1, mousePositionWorld2);
}

void UserInterface::render()
{
	if (hoverObject)
	{
		objectPanel->render(hoverObject);
	}
}