#include "Ship.h"
#include "Geometry.h"
#include "World.h"
#include "Renderer.h"
#include "Projectile.h"
#include "Surface.h"
#include <glm\gtx\transform.hpp>

Ship::Ship(World* world) : WorldObject(world)
{
	type = SHIP;
	order = MOVE;

	shieldMatrix = glm::mat4(5.5, 0.0, 0.0, 0.0,
							 0.0, 2.0, 0.0, 0.0,
							 0.0, 0.0, 10.0, 0.0,
							 0.0, 0.0, -1.5, 1.0);
	//shieldMatrix = glm::mat4();
	inverseShieldMatrix = glm::inverse(shieldMatrix);

	forwardAcceleration = 0.1f / 3600;
	forwardDeceleration = 0.1f / 3600;
	forwardMaximum = 1.0f / 60;

	//pitch
	rotateXAcceleration = 0.005f / 3600;
	rotateXMaximum = 0.05f / 60;

	//yaw
	rotateYAcceleration = 0.005f / 3600;
	rotateYMaximum = 0.05f / 60;

	//roll
	rotateZAcceleration = 0.005f / 3600;
	rotateZMaximum = 0.05f / 60;

	hull = 200.0f;
	hullMaximum = 200.0f;

	shield = 75.0f;
	shieldMaximum = 100.0f;

	collisionSphere = 8.0f;
	
	glm::vec3 frustum[8];
	frustum[0] = glm::vec3(0.01f, 0.01f, 8.0f);
	frustum[1] = glm::vec3(-0.01f, 0.01f, 8.0f);
	frustum[2] = glm::vec3(-0.01f, -0.01f, 8.0f);
	frustum[3] = glm::vec3(0.01f, -0.01f, 8.0f);
	frustum[4] = glm::vec3(4.5f, 1.3f, -8.0f);
	frustum[5] = glm::vec3(-4.5f, 1.3f, -8.0f);
	frustum[6] = glm::vec3(-4.5f, -1.2f, -8.0f);
	frustum[7] = glm::vec3(4.5f, -1.2f, -8.0f);

	Geometry::constructFrustum(collisionPlanes, frustum);

	targetPosition = glm::vec4(-5.0f, 2.0f, -8.0f, 1.0f);
}

Ship::~Ship()
{

}

void Ship::render()
{
	WorldObject::render();

	Renderer* renderer = world->renderer;
	OrderIndependentTransparencyShader* orderIndependentTransparencyShader = renderer->orderIndependentTransparencyShader;

	if (shield > 0.0f)
	{
		//Use the shield shader to render the shield
		glm::mat4 matrix = modelMatrix * shieldMatrix;
		orderIndependentTransparencyShader->renderShield(matrix, shieldMatrix, hits);
	}
}

void Ship::update()
{
	//Work out the distance between us and our target, and our stopping distance
	glm::vec4 positionToTarget = targetPosition - position;
	float length = glm::length(positionToTarget);
	float stoppingDistance = getStoppingDistance();

	if (order == MOVE)
	{
		//If we're close to our target, start decelerating
		if (length < stoppingDistance)
		{
			targetPosition = position;
			length = 0;

			//Once we've stopped, make sure we carry on facing the same direction.
			if (getForwardSpeed() < forwardDeceleration)
			{
				order = NONE;
				targetPosition = modelMatrix * glm::vec4(0.0, 0.0, 1.0, 1.0);
			}
		}
	}

	//Find the target position relative to us, so we know which way we need to turn
	glm::vec4 relativeTargetPosition = inverseModelMatrix * targetPosition;

	float angleX = 0;
	float angleY = 0;
	if (length > stoppingDistance)
	{
		angleX = atan2(relativeTargetPosition.y, relativeTargetPosition.z);
		angleY = atan2(relativeTargetPosition.x, relativeTargetPosition.z);

		if (angleX < World::PI / 1800) angleX = 0;
		if (angleY < World::PI / 1800) angleY = 0;
	}
	
	//If our target is behind us, then don't angle our pitch yet
	double piBy2 = World::PI / 2;
	if (angleX > piBy2 || angleX < -piBy2)
	{
		angleX = 0.0f;
	}

	//To keep the ship upright, find how far off we are
	float angleZ = getAngleZToUpright(position);

	float rotationRateX = getRotationRateX();
	float rotationRateY = getRotationRateY();
	float rotationRateZ = getRotationRateZ();

	float rotateYToAccelerate = getRotationAcceleration(rotationRateY, angleY, 0.0f, rotateYAcceleration, rotateYMaximum);
	float rotateXToAccelerate = getRotationAcceleration(rotationRateX, angleX, 0.0f, rotateXAcceleration, rotateXMaximum);
	float rotateZToAccelerate = getRotationAcceleration(rotationRateZ, angleZ, 0.0f, rotateZAcceleration, rotateZMaximum);

	float forwardToAccelerate;
	if (order == MOVE)
	{
		forwardToAccelerate = getAcceleration(angleY, angleX, length, stoppingDistance);
	}
	else
	{
		forwardToAccelerate = getAcceleration(angleY, angleX, 0, stoppingDistance);
	}

	/*float deltaTime = world->deltaTime;
	movementMatrix *= glm::rotate(rotateYToAccelerate * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	movementMatrix *= glm::rotate(rotateXToAccelerate * deltaTime, glm::vec3(-1.0f, 0.0f, 0.0f));
	movementMatrix *= glm::rotate(rotateZToAccelerate * deltaTime, glm::vec3(0.0f, 0.0f, -1.0f));
	movementMatrix *= glm::translate(glm::vec3(0.0f, 0.0f, forwardToAccelerate) * deltaTime);*/
	movementMatrix *= glm::rotate(rotateYToAccelerate, glm::vec3(0.0f, 1.0f, 0.0f));
	movementMatrix *= glm::rotate(rotateXToAccelerate, glm::vec3(-1.0f, 0.0f, 0.0f));
	movementMatrix *= glm::rotate(rotateZToAccelerate, glm::vec3(0.0f, 0.0f, -1.0f));
	movementMatrix *= glm::translate(glm::vec3(0.0f, 0.0f, forwardToAccelerate));

	if (rotationRateX == 0
		&& rotationRateY == 0
		&& rotationRateZ == 0
		&& rotateYToAccelerate == 0
		&& rotateXToAccelerate == 0
		&& rotateZToAccelerate == 0
		&& getForwardSpeed() == 0)
	{
		movementMatrix = glm::mat4();
	}

	/*rotationRateX = getRotationRateX();
	rotationRateY = getRotationRateY();
	rotationRateZ = getRotationRateZ();
	float forwardSpeed = getForwardSpeed();

	glm::mat4 movementMatrixDelta;
	movementMatrixDelta *= glm::rotate(rotationRateY * deltaTime, glm::vec3( 0.0f, 1.0f, 0.0f));
	movementMatrixDelta *= glm::rotate(rotationRateX * deltaTime, glm::vec3(-1.0f, 0.0f, 0.0f));
	movementMatrixDelta *= glm::rotate(rotationRateZ * deltaTime, glm::vec3( 0.0f, 0.0f,-1.0f));
	movementMatrixDelta *= glm::translate(glm::vec3(0.0f, 0.0f, forwardSpeed) * deltaTime);

	modelMatrix *= movementMatrixDelta;*/
	
	WorldObject::update();

	for (std::vector<ShieldHit>::iterator i = hits.begin(); i != hits.end(); )
	{
		ShieldHit &hit = *i;
		hit.size -= 0.005f;
		if (hit.size <= 0)
		{
			i = hits.erase(i);
		}
		else
		{
			i++;
		}
	}

	//Bone &frontBone = bones[1];
	//frontBone.relative = glm::rotate(0.01f, glm::vec3(1.0f, 0.0f, 0.0f)) * frontBone.relative;

	surface->boneMatricesPointer = &boneTransforms[0];
	surface->modelBonesPointer = &bones[0];
	surface->recalculateModelBoneMatrices();
}

float Ship::getRotationRateX()
{
	float m21 = movementMatrix[2][1];
	float m22 = movementMatrix[2][2];
	return atan2(m21, m22);
}

float Ship::getRotationRateY()
{
	float m20 = movementMatrix[2][0];
	float m21 = movementMatrix[2][1];
	float m22 = movementMatrix[2][2];
	float y = m20;
	float x = sqrt((m21 * m21) + (m22 * m22));
	return atan2(y, x);
}

float Ship::getRotationRateZ()
{
	float m00 = movementMatrix[0][0];
	float m10 = movementMatrix[1][0];
	return atan2(m10, m00);
}

float Ship::getForwardSpeed()
{
	return movementMatrix[3][2];
}

float Ship::getStoppingDistance()
{
	float forwardSpeed = getForwardSpeed();
	float stoppingDistance = (forwardSpeed * forwardSpeed) / (2 * forwardDeceleration);
	if (stoppingDistance < 0.1f)
	{
		stoppingDistance = 0.1f;
	}
	return stoppingDistance;
}

float Ship::getAngleZToUpright(glm::vec4 position)
{
	glm::vec4 aboveTarget = position + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 relativeAboveTarget = inverseModelMatrix * aboveTarget;
	return atan2(relativeAboveTarget.x, relativeAboveTarget.y);
}

float Ship::getAcceleration(float angleY, float angleX, float length, float stoppingDistance)
{
	float forwardSpeed = getForwardSpeed();

	//If we're vaguely facing the target, and we're far from the target, then accelerate. Otherwise, decelerate so we don't go the wrong way
	if (abs(angleY) < World::PI/4 && abs(angleX) < World::PI/18 && length > stoppingDistance)
	{
		//If our full acceleration would increase our speed over the maximum, then just accelerate to bring our speed to the maximum
		//if (forwardSpeed + forwardAcceleration * world->deltaTime < forwardMaximum)
		if (forwardSpeed + forwardAcceleration < forwardMaximum)
		{
			return forwardAcceleration;
		}
		else
		{
			return forwardMaximum - forwardSpeed;
		}
	}
	else
	{
		//If our full deceleration would decrease our speed to make us move backwards, then just decelerate to bring us to a stop
		//if (forwardSpeed > forwardDeceleration * world->deltaTime)
		if (forwardSpeed > forwardDeceleration)
		{
			return -forwardDeceleration;
		}
		else
		{
			return -forwardSpeed;
		}
	}
}

double Ship::getRotationAcceleration(double currentRotationSpeed, double currentAngle, double targetAngle, double rotationAcceleration, double rotationSpeedMaximum)
{
	double stoppingDistance = (currentRotationSpeed * currentRotationSpeed) / (2 * rotationAcceleration);
	if (stoppingDistance < World::PI/1800)
	{
		stoppingDistance = World::PI /1800;
	}

	//Calculate how much we can change our rotation this frame
	//float rotationAccelerationDelta = rotationAcceleration * world->deltaTime;

	if (abs(currentAngle - targetAngle) > stoppingDistance)
	{
		if (currentAngle > targetAngle)
		{
			//if (currentRotationSpeed + rotationAccelerationDelta < rotationSpeedMaximum)
			if (currentRotationSpeed + rotationAcceleration < rotationSpeedMaximum)
			{
				return rotationAcceleration;
			}
			else
			{
				//return (rotationSpeedMaximum - currentRotationSpeed) / world->deltaTime;
				return rotationSpeedMaximum - currentRotationSpeed;
			}
		}
		else if (currentAngle < targetAngle)
		{
			//if (currentRotationSpeed - rotationAccelerationDelta > -rotationSpeedMaximum)
			if (currentRotationSpeed - rotationAcceleration > -rotationSpeedMaximum)
			{
				return -rotationAcceleration;
			}
			else
			{
				//return (-rotationSpeedMaximum - currentRotationSpeed) / (world->deltaTime * world->deltaTime);
				return -rotationSpeedMaximum - currentRotationSpeed;
			}
		}
	}
	else
	{
		//if (currentRotationSpeed > rotationAccelerationDelta)
		if (currentRotationSpeed > rotationAcceleration)
		{
			return -rotationAcceleration;
		}
		//else if (currentRotationSpeed < -rotationAccelerationDelta)
		else if (currentRotationSpeed < -rotationAcceleration)
		{
			return rotationAcceleration;
		}
		else
		{
			//return -currentRotationSpeed / world->deltaTime;
			return -currentRotationSpeed;
		}
	}

	return 0;
}

void Ship::collisionWithProjectile(Projectile* projectile)
{
	glm::vec4 end1 = inverseModelMatrix * projectile->previousPosition;
	glm::vec4 end2 = inverseModelMatrix * projectile->position;
	
	if (shield > 0)
	{
		glm::vec3 end1Shield = glm::vec3(inverseShieldMatrix * end1);
		glm::vec3 end2Shield = glm::vec3(inverseShieldMatrix * end2);

		glm::vec4 intersection;
		//if (Geometry::getLineEllipsoidIntersection(end1, end2, 4.5, 2.0, 10.0, intersection))
		if (Geometry::getLineSphereIntersection(end1Shield, end2Shield, intersection))
		{
			intersection = shieldMatrix * intersection;

			projectile->markForDestruction = true;
			shield -= projectile->shieldDamage;
			ShieldHit newHit{ glm::vec3(intersection), 0.1f };
			hits.push_back(newHit);
			return;
		}
	}

	if (Geometry::isLineInSphere(glm::vec4(), end1, end2, collisionSphere)
	  && Geometry::isLineInFrustum(end1, end2, collisionPlanes))
	{
		projectile->markForDestruction = true;
		hull -= projectile->hullDamage;
	}
}