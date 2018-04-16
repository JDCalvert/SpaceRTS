#include "World.h"
#include "Surface.h"
#include "WorldObject.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Controller.h"
#include "Ship.h"
#include "Geometry.h"
#include "Projectile.h"

const double World::PI = 4 * atan(1.0);

World::World(Renderer* renderer)
{
	this->renderer = renderer;

	GLuint spaceTexture = ResourceLoader::loadDDS("Graphics/spaceTextureBig.DDS");
	GLuint normalTextureBlank = ResourceLoader::loadDDS("Graphics/normalTextureBlank.DDS");
	GLuint specularTextureBlack = ResourceLoader::loadDDS("Graphics/specularTextureBlack.DDS");
	GLuint metalTexture = ResourceLoader::loadDDS("Graphics/metalTexture.DDS");
	GLuint normalTextureInOut = ResourceLoader::loadDDS("Graphics/normalTextureInOut.DDS");
	GLuint specularTextureWhite = ResourceLoader::loadDDS("Graphics/specularTextureWhite.DDS");

	//Set up a cube as a test object
	Surface* cubeSurface = new Surface();
	cubeSurface->loadFromFile("Models/cube.objComplete");
	cubeSurface->setTextures(metalTexture, normalTextureInOut, metalTexture);
	cubeSurface->setAmbientColour(glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
	cubeSurface->setDiffuseColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
	cubeSurface->setUpColourPointers();

	projectileSurface = new Surface();
	projectileSurface->loadFromFile("Models/sphere.objComplete", 0.02f);
	projectileSurface->setTextures(specularTextureWhite, normalTextureBlank, specularTextureBlack);
	projectileSurface->setAmbientColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	projectileSurface->setDiffuseColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	projectileSurface->setUpColourPointers();

	//Set up the sky sphere
	Surface* skySphere = new Surface();
	skySphere->loadFromFile("Models/sphere.objComplete", 100.0f);
	skySphere->setTextures(spaceTexture, normalTextureBlank, specularTextureBlack);
	skySphere->setAmbientColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	skySphere->setDiffuseColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	skySphere->setUpColourPointers();

	sky = new WorldObject(this);
	sky->setSurface(skySphere);

	Surface* lightSphere = new Surface();
	lightSphere->loadFromFile("Models/sphere.objComplete");
	lightSphere->setTextures(specularTextureWhite, normalTextureBlank, specularTextureBlack);
	lightSphere->setAmbientColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	lightSphere->setDiffuseColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	lightSphere->setUpColourPointers();

	//Set up a cube as a test object
	/*Surface* cubeSurface = new Surface();
	cubeSurface->loadFromFile("Models/cube.objComplete", "Models/cube.bone");
	cubeSurface->setTextures(metalTexture, normalTextureInOut, metalTexture);
	cubeSurface->setAmbientColour(glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
	cubeSurface->setDiffuseColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	cubeSurface->setUpColourPointers();*/

	WorldObject* cube = new WorldObject(this);
	cube->setSurface(cubeSurface);
	objects.push_back(cube);

	Surface* triangleSurface = new Surface();
	triangleSurface->loadFromFile("Models/triangle.objComplete");
	triangleSurface->setTextures(metalTexture, normalTextureBlank, metalTexture);
	triangleSurface->setAmbientColour(glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
	triangleSurface->setDiffuseColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	triangleSurface->setUpColourPointers();

	Surface* capitalShipSurface = new Surface();
	capitalShipSurface->loadFromFile("Models/CapitalShip.objComplete");
	capitalShipSurface->setTextures(metalTexture, normalTextureBlank, metalTexture);
	capitalShipSurface->setAmbientColour(glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
	capitalShipSurface->setDiffuseColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	capitalShipSurface->setUpColourPointers();

	Ship* ship = new Ship(this);
	ship->setSurface(capitalShipSurface);
	ship->setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
	objects.push_back(ship);

	Surface* sphereSurface = new Surface();
	sphereSurface->loadFromFile("Models/sphere.objComplete");
	sphereSurface->setTextures(metalTexture, normalTextureBlank, metalTexture);
	sphereSurface->setAmbientColour(glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
	sphereSurface->setDiffuseColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	sphereSurface->setUpColourPointers();

	WorldObject* light = new WorldObject(this);
	light->setSurface(lightSphere);
	light->setPosition(glm::vec3(10.0f, 8.0f, 15.0f));
	objects.push_back(light);

	WorldObject* xMarker = new WorldObject(this);
	xMarker->setSurface(cubeSurface);
	xMarker->setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
	objects.push_back(xMarker);

	WorldObject* yMarker = new WorldObject(this);
	yMarker->setSurface(sphereSurface);
	yMarker->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	objects.push_back(yMarker);

	WorldObject* zMarker = new WorldObject(this);
	zMarker->setSurface(triangleSurface);
	zMarker->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	objects.push_back(zMarker);
}

World::~World()
{

}

void World::update()
{
	Controller* controller = renderer->controller;
	glm::vec3 cameraPosition = controller->getCameraPosition();
	sky->setPosition(cameraPosition);

	//Just for now, create a projectile when we click
	if (controller->leftMouseButtonPressed)
	{
		glm::vec3 position = controller->getCameraPosition();
		glm::vec3 right = controller->getCameraRight();
		glm::vec3 up = controller->getCameraUp();
		glm::vec3 forward = controller->getCameraForward();
		glm::mat4 projectileMatrix = glm::mat4(glm::vec4(-right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(position, 1.0f));

		Projectile* projectile = new Projectile(this);
		projectile->modelMatrix = projectileMatrix;
		projectile->position = glm::vec4(position, 1.0);
		projectile->previousPosition = glm::vec4(position, 1.0);
		projectile->setSurface(projectileSurface);
		objects.push_back(projectile);
	}

	//Go through our objects and update them.
	for (auto i = objects.begin(); i != objects.end(); )
	{
		//Update our object
		WorldObject* object = *i;
		object->update();

		if (object->markForDestruction)
		{
			//If the object is marked for destruction, then delete the object and remove it from the list.
			//The object should take care of cleaning up when it's deleted or marked for destruction.
			delete object;
			i = objects.erase(i);
		}
		else
		{
			//If the object hasn't been deleted, then move on to the next object in the list
			i++;
		}
	}

	for (auto i = objects.begin(); i != objects.end(); i++)
	{
		WorldObject* ship = *i;
		if (ship->type == SHIP)
		{
			for (auto j = objects.begin(); j != objects.end(); j++)
			{
				WorldObject* object = *j;
				if (object->type == PROJECTILE)
				{
					Projectile* projectile = (Projectile*)object;
					ship->collisionWithProjectile(projectile);
				}
			}
		}
	}
}

void World::render()
{
	sky->render();

	//Render each of our objects in our linked list
	for (auto i = objects.begin(); i != objects.end(); i++)
	{
		WorldObject* object = *i;
		object->render();
	}
}

void World::getLightSources(std::vector<glm::vec3> lightSources)
{
}

WorldObject* World::getHoverObject(glm::vec4 mousePosition1, glm::vec4 mousePosition2)
{
	for (auto i = objects.begin(); i != objects.end(); i++)
	{
		WorldObject* object = *i;
		if (object->collisionWithLine(mousePosition1, mousePosition2))
		{
			return object;
		}
	}

	//If we're not hovering over anything, return null
	return nullptr;
}