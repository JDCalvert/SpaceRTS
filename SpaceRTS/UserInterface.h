#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class Renderer;
class UserInterfacePanel;
class ObjectPanel;
class WorldObject;
class World;

class UserInterface
{
	public:
	UserInterface(Renderer* renderer, World* world);
	~UserInterface();

	Renderer* renderer;
	World* world;

	ObjectPanel* objectPanel;

	WorldObject* hoverObject;

	void render();
	void update();
};

#endif