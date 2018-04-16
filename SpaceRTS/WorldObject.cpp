#include "WorldObject.h"
#include "Surface.h"
#include "Renderer.h"
#include "World.h"
#include "Geometry.h"
#include "OrderIndependentTransparencyShader.h"
#include "Projectile.h"

WorldObject::WorldObject(World* world)
{
	type = WORLD_OBJECT;

	this->world = world;

	markForDestruction = false;

	movementMatrix = glm::mat4();
	calculateInverseModelMatrixAndPosition();

	collisionSphere = 1.0f;

	glm::vec3 frustum[8];
	frustum[0] = glm::vec3( 0.01f, 0.01f, 1.0f);
	frustum[1] = glm::vec3(-0.01f, 0.01f, 1.0f);
	frustum[2] = glm::vec3(-0.01f,-0.01f, 1.0f);
	frustum[3] = glm::vec3( 0.01f,-0.01f, 1.0f);
	frustum[4] = glm::vec3( 0.50f, 0.01f,-1.0f);
	frustum[5] = glm::vec3(-0.50f, 0.01f,-1.0f);
	frustum[6] = glm::vec3(-0.50f,-0.01f,-1.0f);
	frustum[7] = glm::vec3( 0.50f,-0.01f,-1.0f);

	Geometry::constructFrustum(collisionPlanes, frustum);
}

void WorldObject::setSurface(Surface* surface)
{
	this->surface = surface;

	bones.clear();
	boneTransforms.clear();
	for (unsigned int i = 0; i < surface->bones.size(); i++)
	{
		BindBone bone = surface->bones[i];
		bones.push_back(Bone(bone.relative, bone.parent));
		boneTransforms.push_back(bone.absolute * bone.inverseBind);
	}
}

void WorldObject::update()
{
	modelMatrix *= movementMatrix;
	calculateInverseModelMatrixAndPosition();
}

void WorldObject::render()
{
	Renderer* renderer = world->renderer;
	
	surface->boneMatricesPointer = &boneTransforms[0];
	surface->modelBonesPointer = &bones[0];
	
	//Use the order independent tranparency shader to render the model
	OrderIndependentTransparencyShader* orderIndependentTransparencyShader = renderer->orderIndependentTransparencyShader;
	orderIndependentTransparencyShader->renderSurface(modelMatrix, surface);
}

void WorldObject::setPosition(glm::vec3 position)
{
	modelMatrix[3] = glm::vec4(position, 1.0f);
	calculateInverseModelMatrixAndPosition();
}

void WorldObject::calculateInverseModelMatrixAndPosition()
{
	inverseModelMatrix = glm::inverse(modelMatrix);
	previousPosition = position;
	position = modelMatrix[3];
}

void WorldObject::collisionWithProjectile(Projectile* projectile)
{
	glm::vec4 end1 = inverseModelMatrix * projectile->previousPosition;
	glm::vec4 end2 = inverseModelMatrix * projectile->position;
	
	if (Geometry::isLineInSphere(glm::vec4(), end1, end2, collisionSphere)
	  && Geometry::isLineInFrustum(end1, end2, collisionPlanes))
	{
		//The projectile has hit the object. Mark the projectile for deletion
		projectile->markForDestruction = true;
	}
}

bool WorldObject::collisionWithLine(glm::vec4 end1, glm::vec4 end2)
{
	end1 = inverseModelMatrix * end1;
	end2 = inverseModelMatrix * end2;

	if (Geometry::isLineInSphere(glm::vec4(0.0f), end1, end2, collisionSphere)
	  && Geometry::isLineInFrustum(end1, end2, collisionPlanes))
	{
		return true;
	}

	return false;
}