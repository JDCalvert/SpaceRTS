#ifndef WORLDOBJECT
#define WORLDOBJECT

#include <glm/glm.hpp>
#include <vector>

#include "Surface.h"

enum ObjectType
{
	WORLD_OBJECT,
	SHIP,
	PROJECTILE
};

class World;
class Projectile;

class WorldObject
{
	public:
	World* world;

	ObjectType type;
	
	Surface* surface;
	std::vector<Bone> bones;
	std::vector<glm::mat4> boneTransforms;

	glm::mat4 movementMatrix;

	glm::mat4 modelMatrix;
	glm::mat4 inverseModelMatrix;
	glm::vec4 position;
	glm::vec4 previousPosition;
	
	float collisionSphere;
	float collisionPlanes[6][4];

	float hull;
	float hullMaximum;
	
	float shield;
	float shieldMaximum;
	float shieldRechargeOn;
	float shieldRechargeOff;

	bool markForDestruction;

	WorldObject(World* world);
	
	void setSurface(Surface* surface);
	virtual void render();
	virtual void update();
	void setPosition(glm::vec3 position);
	void calculateInverseModelMatrixAndPosition();
	
	virtual bool collisionWithLine(glm::vec4 end1, glm::vec4 end2);
	virtual void collisionWithProjectile(Projectile* projectile);
};

#endif