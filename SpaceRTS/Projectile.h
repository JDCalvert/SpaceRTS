#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "WorldObject.h"

class Projectile : public WorldObject
{
	public:
	Projectile(World* world);
	~Projectile();

	//Regular ballistic projectiles cannot change speed or direction
	float speed;

	int lifeTime;
	int age;

	float shieldDamage;
	float hullDamage;

	void update();
	void collision();
};

#endif