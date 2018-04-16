#ifndef ORDER_INDEPENDENT_TRANSPARENCY_SHADER
#define ORDER_INDEPENDENT_TRANSPARENCY_SHADER

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include "Shader.h"

class Renderer;
class Surface;

struct ShieldHit
{
	glm::vec3 position;
	float size;
};

class OrderIndependentTransparencyShader : Shader
{
	public:
	OrderIndependentTransparencyShader(Renderer* renderer);

	//Renderer* renderer;

	GLuint headPointerTexture;
	GLuint headPointerInitialiser;
	GLuint atomicCounterBuffer;
	GLuint fragmentStorageBuffer;
	GLuint fragmentStorageTexture;

	GLuint opaqueFramebufferId;
	GLuint opaqueFramebufferColour;
	GLuint opaqueFramebufferDepth;

	GLuint opaqueDepthTexture;
	GLuint opaqueDepthInitialiser;

	//GLuint programId;
	GLuint vertexPositionId;
	GLuint textureCoordinateId;
	GLuint vertexNormalId;
	GLuint vertexTangentId;
	GLuint vertexBitangentId;
	GLuint vertexBonesId;

	GLuint ambientId;
	GLuint diffuseId;
	GLuint modelViewProjectionId;
	GLuint modelMatrixId;
	GLuint diffuseMapId;
	GLuint normalMapId;
	GLuint specularMapId;
	GLuint lightPositionId;
	GLuint cameraPositionId;

	GLuint vertexPositionBufferId;
	GLuint textureCoordinatesBufferId;
	GLuint vertexNormalBufferId;
	GLuint vertexTangentBufferId;
	GLuint vertexBitangentBufferId;
	GLuint vertexBoneBufferId;
	GLuint indicesBufferId;

	GLuint boneBufferId;

	GLuint shieldProgramId;
	GLuint shieldVertexPositionId;
	GLuint shieldModelViewProjectionMatrixId;
	GLuint shieldModelMatrixId;
	GLuint shieldShieldMatrixId;
	GLuint shieldCameraPositionId;
	GLuint shieldHitId;

	GLuint shieldVertexBufferId;
	GLuint shieldIndexBufferId;
	GLuint shieldHitBufferId;

	unsigned int shieldNumIndices;

	GLuint blenderProgramId;
	GLuint blenderVertexPositionId;
	
	GLuint blenderVertexPositionBufferId;

	void initialise();
	void initialiseShaders();
	void initialiseMainShader();
	void initialiseShieldShader();
	void initialiseBlenderShader();
	void initialiseForScreenSize();
	void initialiseFrame();

	void renderSurface(glm::mat4 modelMatrix, Surface* surface);
	void renderShield(glm::mat4 modelMatrix, glm::mat4 shieldMatrix, std::vector<ShieldHit> &hits);

	void renderFrame();
};

#endif
