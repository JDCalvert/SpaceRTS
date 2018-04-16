#include "Projectile.h"
#include "World.h"

#include <glm\gtx\transform.hpp>

/**
 * Base class for projectiles. Has a constant forward speed from its creation, and cannot turn.
 */
Projectile::Projectile(World* world) : WorldObject(world)
{
	type = PROJECTILE;

	shieldDamage = 1.0f;
	hullDamage = 10.0f;

	age = 0;
	lifeTime = 600;

	glm::vec3 forwardMatrix = glm::vec3(0.0f, 0.0f, 5.0f) * ((float)1/60);

	movementMatrix = glm::mat4();
	movementMatrix *= glm::translate(forwardMatrix);
}

Projectile::~Projectile()
{
}

void Projectile::update()
{
	WorldObject::update();

	age++;
	if (age >= lifeTime)
	{
		markForDestruction = true;
	}
}