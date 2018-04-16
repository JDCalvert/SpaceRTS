#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <list>
#include <glm\fwd.hpp>

class WorldObject;
class Renderer;
class Ship;
class Surface;

class World
{
	public:
	World(Renderer* renderer);
	~World();
		
	Renderer* renderer;

	static const double PI;

	Surface* projectileSurface;

	//Keep track of the sky object because it's special
	WorldObject* sky;

	//Keep all the objects in a linked list
	std::list<WorldObject*> objects;

	void update();
	void render();
	void getLightSources(std::vector<glm::vec3> lightSources);
	WorldObject* getHoverObject(glm::vec4 mousePosition1, glm::vec4 mousePosition2);
};

#endif