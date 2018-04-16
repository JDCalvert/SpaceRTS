#include "UserInterfacePanel.h"
#include "UserInterface.h"

UserInterfacePanel::UserInterfacePanel(UserInterface* userInterface)
{
	this->userInterface = userInterface;
}

UserInterfacePanel::~UserInterfacePanel()
{
}

void UserInterfacePanel::render()
{
	Renderer* renderer = userInterface->renderer;

	renderer->renderUserInterfacePanel(this);
}