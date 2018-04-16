#include "ObjectPanel.h"
#include "UserInterface.h"
#include "WorldObject.h"

ObjectPanel::ObjectPanel(UserInterface* userInterface) : UserInterfacePanel(userInterface)
{

}

ObjectPanel::~ObjectPanel()
{
}

void ObjectPanel::render(WorldObject* object)
{
	UserInterfacePanel::render();

	Renderer* renderer = userInterface->renderer;

	float xPosition = position.x;
	float yPosition = position.y;

	renderer->renderText("Ship", xPosition + 0.01f, yPosition + 0.01f, 0.03f);

	std::vector<glm::vec2> positions;
	std::vector<glm::vec2> dimensions;
	std::vector<glm::vec4> colours;

	float hullNormalised = object->hull / object->hullMaximum;
	float shieldNormalised = object->shield / object->shieldMaximum;

	float shieldBarPositionX = xPosition + 0.01f;
	float shieldBarPositionY = yPosition + 0.05f;
	float shieldBarDimensionX = width - 0.02f;
	float shieldBarDimensionY = 0.03f;
	float hullBarBorder = 0.003f;
	float hullBarPositionX = shieldBarPositionX + hullBarBorder;
	float hullBarPositionY = shieldBarPositionY + hullBarBorder;
	float hullBarDimensionXMax = shieldBarDimensionX - 2*hullBarBorder;
	float hullBarDimensionY = shieldBarDimensionY - 2*hullBarBorder;

	//Black bar
	positions.push_back(glm::vec2(shieldBarPositionX, shieldBarPositionY));
	dimensions.push_back(glm::vec2(shieldBarDimensionX, shieldBarDimensionY));
	colours.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Hull bar
	positions.push_back(glm::vec2(hullBarPositionX, hullBarPositionY));
	dimensions.push_back(glm::vec2(hullBarDimensionXMax * hullNormalised, hullBarDimensionY));
	colours.push_back(glm::vec4(0.0f, 0.75f, 0.0f, 1.0f));

	//Shield bar
	positions.push_back(glm::vec2(shieldBarPositionX, shieldBarPositionY));
	dimensions.push_back(glm::vec2(shieldBarDimensionX * shieldNormalised, shieldBarDimensionY));
	colours.push_back(glm::vec4(0.0f, 0.75f, 0.75f, 0.5f));

	renderer->renderBars(positions, dimensions, colours);
}