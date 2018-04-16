#ifndef SHIP_H
#define SHIP_H
#include "WorldObject.h"
#include "OrderIndependentTransparencyShader.h"

#include <vector>

enum Order {NONE, MOVE};

class Ship : public WorldObject
{
	public:
	Ship(World* world);
	~Ship();

	glm::mat4 shieldMatrix;
	glm::mat4 inverseShieldMatrix;
	std::vector<ShieldHit> hits;

	Order order = NONE;

	glm::vec4 targetPosition;
	glm::vec4 facePosition;

	float forwardAcceleration;
	float forwardDeceleration;
	float forwardMaximum;

	//pitch
	double rotateXAcceleration;
	double rotateXMaximum;

	//yaw
	double rotateYAcceleration;
	double rotateYMaximum;

	//roll
	double rotateZAcceleration;
	double rotateZMaximum;

	void update();
	void render();

	void collisionWithProjectile(Projectile* projectile);

	float getStoppingDistance();
	float getAngleZToUpright(glm::vec4 position);

	float getForwardSpeed();
	float getRotationRateX();
	float getRotationRateY();
	float getRotationRateZ();
	float getAcceleration(float angleY, float angleX, float length, float stoppingDistance);
	double getRotationAcceleration(double currentRotationSpeed, double currentAngle, double targetAngle, double rotationAcceleration, double rotationSpeedMaximum);
};

#endif