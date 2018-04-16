#ifndef OBJECT_PANEL_H
#define OBJECT_PANEL_H

#include "UserInterfacePanel.h"

class ObjectPanel : public UserInterfacePanel
{
	public:
	ObjectPanel(UserInterface* userInterface);
	~ObjectPanel();

	void render(WorldObject* object);
};

#endif OBJECT_PANEL_H