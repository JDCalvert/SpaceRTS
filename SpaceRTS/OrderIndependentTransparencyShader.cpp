#include "OrderIndependentTransparencyShader.h"
#include "Renderer.h"
#include "Controller.h"
#include "Surface.h"
#include "ResourceLoader.h"
#include "Geometry.h"
#include "Shader.h"

#include <sstream>

OrderIndependentTransparencyShader::OrderIndependentTransparencyShader(Renderer* renderer) : Shader(renderer) {}

void OrderIndependentTransparencyShader::initialiseShaders()
{
	initialiseMainShader();
	initialiseShieldShader();
	initialiseBlenderShader();
}

void OrderIndependentTransparencyShader::initialiseMainShader()
{
	//Initialise the main shader
	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "OrderIndependentTransparencyShader.vert"},
		{GL_FRAGMENT_SHADER, "OrderIndependentTransparencyShader.frag"}
	};
	programId = ResourceLoader::loadShaders(shaders, 2);

	vertexPositionId		= glGetAttribLocation(programId, "vertexPosition");
	textureCoordinateId		= glGetAttribLocation(programId, "vertexTextureCoordinates");
	vertexNormalId			= glGetAttribLocation(programId, "vertexNormal");
	vertexTangentId			= glGetAttribLocation(programId, "vertexTangent");
	vertexBitangentId		= glGetAttribLocation(programId, "vertexBitangent");
	vertexBonesId			= glGetAttribLocation(programId, "vertexBones");

	ambientId				= glGetUniformLocation(programId, "ambient");
	diffuseId				= glGetUniformLocation(programId, "diffuse");
	modelViewProjectionId	= glGetUniformLocation(programId, "modelViewProjectionMatrix");
	modelMatrixId			= glGetUniformLocation(programId, "modelMatrix");
	diffuseMapId			= glGetUniformLocation(programId, "diffuseMap");
	normalMapId				= glGetUniformLocation(programId, "normalMap");
	specularMapId			= glGetUniformLocation(programId, "specularMap");
	lightPositionId			= glGetUniformLocation(programId, "lightPosition");
	cameraPositionId		= glGetUniformLocation(programId, "cameraPosition");

	boneBufferId			= glGetProgramResourceIndex(shieldProgramId, GL_SHADER_STORAGE_BLOCK, "boneBuffer");

	glGenBuffers(1, &vertexPositionBufferId);
	glGenBuffers(1, &vertexNormalBufferId);
	glGenBuffers(1, &vertexTangentBufferId);
	glGenBuffers(1, &vertexBitangentBufferId);
	glGenBuffers(1, &vertexBoneBufferId);
	glGenBuffers(1, &indicesBufferId);
}

void OrderIndependentTransparencyShader::initialiseBlenderShader()
{
	//Initialise the blender shader
	ShaderInfo shaders[] = 
	{
		{GL_VERTEX_SHADER, "OrderIndependentTransparencyBlender.vert"},
		{GL_FRAGMENT_SHADER, "OrderIndependentTransparencyBlender.frag"}
	};
	blenderProgramId = ResourceLoader::loadShaders(shaders, 2);

	blenderVertexPositionId	= glGetAttribLocation(blenderProgramId, "vertexPosition");

	//Define the coordinates of the rectangle that will cover the whole screen
	static const GLfloat wholeFrameCoords[] =
	{
		-1.0f, 1.0f, //Top left
		-1.0f,-1.0f, //Bottom left
		 1.0f, 1.0f, //Top right
		 1.0f,-1.0f  //Bottom right
	};

	//Generate the buffer to store these coordinates in graphics memory
	glGenBuffers(1, &blenderVertexPositionBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, blenderVertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wholeFrameCoords), &wholeFrameCoords, GL_STATIC_DRAW);
}

void OrderIndependentTransparencyShader::initialiseShieldShader()
{
	//Initalise the shield shader
	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "ShieldShader.vert"},
		{GL_FRAGMENT_SHADER, "ShieldShader.frag"}
	};
	shieldProgramId = ResourceLoader::loadShaders(shaders, 2);

	shieldVertexPositionId			  = glGetAttribLocation(shieldProgramId, "vertexPosition");
	shieldModelViewProjectionMatrixId = glGetUniformLocation(shieldProgramId, "modelViewProjMatrix");
	shieldModelMatrixId				  = glGetUniformLocation(shieldProgramId, "modelMatrix");
	shieldShieldMatrixId			  = glGetUniformLocation(shieldProgramId, "shieldMatrix");
	shieldCameraPositionId			  = glGetUniformLocation(shieldProgramId, "cameraPosition");
	shieldHitId						  = glGetProgramResourceIndex(shieldProgramId, GL_SHADER_STORAGE_BLOCK, "hitBuffer");

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	GLuint verticesSize;
	GLuint indicesSize;
	Geometry::getUnitSphere(vertices, verticesSize, indices, indicesSize, shieldNumIndices);

	glGenBuffers(1, &shieldVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, shieldVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &shieldIndexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shieldIndexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &shieldHitBufferId);
}

void OrderIndependentTransparencyShader::initialise()
{
	initialiseShaders();
	
	//Generate the buffers and textures
	glGenTextures(1, &headPointerTexture); //Texture where each texel points to the first element of a linked list of fragments for that pixel
	glGenBuffers(1, &headPointerInitialiser); //Texture to reset the head pointer texture
	glGenBuffers(1, &fragmentStorageBuffer); //1D buffer to store fragment data for the scene
	glGenTextures(1, &fragmentStorageTexture); //1D texture to access the data in the fragment storage buffer

	//Generate buffer and texture for the opaque fragments
	glGenTextures(1, &opaqueDepthTexture);
	glGenBuffers(1, &opaqueDepthInitialiser);

	//Atomic counter to keep track of the next unused position in the fragment storage buffer
	glGenBuffers(1, &atomicCounterBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCounterBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), 0, GL_DYNAMIC_COPY);

	//Set the filtering on the head pointer texture
	glBindTexture(GL_TEXTURE_2D, headPointerTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, opaqueDepthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Generate a framebuffer
	glGenFramebuffers(1, &opaqueFramebufferId);

	//Generate the texture that will be used as the framebuffer's render buffer
	glGenTextures(1, &opaqueFramebufferColour);
	glBindTexture(GL_TEXTURE_2D, opaqueFramebufferColour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Generate depth buffer
	glGenRenderbuffers(1, &opaqueFramebufferDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffers have failed");
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void OrderIndependentTransparencyShader::initialiseForScreenSize()
{
	//Calculate the new width, height, and number of pixels
	int width = renderer->getWidthAntialiasing();
	int height = renderer->getHeightAntialiasing();
	int totalPixels = renderer->getPixelCountAntialiasing();
	
	//Calculate how big our various buffers need to be
	int averageFragmentsPerPixel = 2;
	int headPointerInitialiserSize = totalPixels * sizeof(GLuint);
	int fragmentStorageBufferSize = averageFragmentsPerPixel * totalPixels * sizeof(glm::uvec4);

	//Recreate the head pointer initialiser with the new width and height
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, headPointerInitialiser);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, headPointerInitialiserSize, 0, GL_STATIC_DRAW);
	GLuint* data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(data, 0x00, headPointerInitialiserSize);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	//Recreate the opaque texture initialiser with the new width and height
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, opaqueDepthInitialiser);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, headPointerInitialiserSize, 0, GL_STATIC_DRAW);
	GLuint* opaqueData = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(opaqueData, 0xFF, headPointerInitialiserSize);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	//Recreate the 1D fragment buffer and give it enough room for a few fragments per pixel
	glBindBuffer(GL_TEXTURE_BUFFER, fragmentStorageBuffer);
	glBufferData(GL_TEXTURE_BUFFER, fragmentStorageBufferSize, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	//Rebind the fragment storage texture to the fragment storage buffer
	glBindTexture(GL_TEXTURE_BUFFER, fragmentStorageTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, fragmentStorageBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	//Reset the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, opaqueFramebufferId);

	glBindTexture(GL_TEXTURE_2D, opaqueFramebufferColour);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, renderer->getWidthAntialiasing(), renderer->getHeightAntialiasing(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, opaqueFramebufferColour, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, opaqueFramebufferDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderer->getWidthAntialiasing(), renderer->getHeightAntialiasing());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, opaqueFramebufferDepth);
}

void OrderIndependentTransparencyShader::initialiseFrame()
{
	int width = renderer->getWidthAntialiasing();
	int height = renderer->getHeightAntialiasing();

	//Reset the head pointer image using the initialiser
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, headPointerInitialiser);
	glBindTexture(GL_TEXTURE_2D, headPointerTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, opaqueDepthInitialiser);
	glBindTexture(GL_TEXTURE_2D, opaqueDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Reset the atomic counter
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicCounterBuffer);
	GLuint data = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &data);

	glBindFramebuffer(GL_FRAMEBUFFER, opaqueFramebufferId);
	glViewport(0, 0, renderer->getWidthAntialiasing(), renderer->getHeightAntialiasing());

	//Clear the colour and depth buffers
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programId);

	//Bind the head pointer texture and fragment storage buffer
	glBindImageTexture(0, fragmentStorageTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32UI);
	glBindImageTexture(1, headPointerTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	glBindImageTexture(2, opaqueDepthTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
}

void OrderIndependentTransparencyShader::renderSurface(glm::mat4 modelMatrix, Surface* surface)
{
	glUseProgram(programId);

	glm::mat4 modelViewMatrix = renderer->viewMatrix * modelMatrix;
	glm::mat4 modelViewProjectionMatrix = renderer->projectionMatrix * modelViewMatrix;

	int verticesSize = surface->verticesSize;
	int textureCoordinatesSize = surface->textureCoordinatesSize;
	int normalsSize = surface->normalsSize;
	int tangentsSize = surface->tangentsSize;
	int bitangentsSize = surface->bitangentsSize;
	int bonesSize = surface->bonesSize;
	int indicesSize = surface->indicesSize;
	int boneMatricesSize = surface->boneMatricesSize;

	glm::vec3* verticesPointer = surface->verticesPointer;
	glm::vec2* textureCoordinatesPointer = surface->textureCoordinatesPointer;
	glm::vec3* normalsPointer = surface->normalsPointer;
	glm::vec3* tangentsPointer = surface->tangentsPointer;
	glm::vec3* bitangentsPointer = surface->bitangentsPointer;
	glm::vec4* bonesPointer = surface->bonesPointer;
	unsigned int* indicesPointer = surface->indicesPointer;
	glm::mat4* boneMatricesPointer = surface->boneMatricesPointer;

	GLfloat* ambientPointer = surface->ambientPointer;
	GLfloat* diffusePointer = surface->diffusePointer;

	int length = surface->length;

	GLuint diffuseMap = surface->diffuseMap;
	GLuint normalMap = surface->normalMap;
	GLuint specularMap = surface->specularMap;

	glm::vec3 lightPosition = glm::vec3(10.0f, 8.0f, 15.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glUniform1i(diffuseMapId, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glUniform1i(normalMapId, 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glUniform1i(specularMapId, 2);

	renderer->bindArrayBufferData(vertexPositionBufferId, verticesSize, verticesPointer);
	renderer->bindArrayBufferData(textureCoordinatesBufferId, textureCoordinatesSize, textureCoordinatesPointer);
	renderer->bindArrayBufferData(vertexNormalBufferId, normalsSize, normalsPointer);
	renderer->bindArrayBufferData(vertexTangentBufferId, tangentsSize, tangentsPointer);
	renderer->bindArrayBufferData(vertexBitangentBufferId, bitangentsSize, bitangentsPointer);
	renderer->bindArrayBufferData(vertexBoneBufferId, bonesSize, bonesPointer);

	renderer->bindElementArrayBufferData(indicesBufferId, indicesSize, indicesPointer);

	renderer->enableVertexAttrib(vertexPositionId, vertexPositionBufferId, 3);
	renderer->enableVertexAttrib(textureCoordinateId, textureCoordinatesBufferId, 2);
	renderer->enableVertexAttrib(vertexNormalId, vertexNormalBufferId, 3);
	renderer->enableVertexAttrib(vertexTangentId, vertexTangentBufferId, 3);
	renderer->enableVertexAttrib(vertexBitangentId, vertexBitangentBufferId, 3);
	renderer->enableVertexAttrib(vertexBonesId, vertexBoneBufferId, 4);

	glUniform4fv(ambientId, 1, ambientPointer);
	glUniform4fv(diffuseId, 1, diffusePointer);

	glUniformMatrix4fv(modelViewProjectionId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniform3fv(lightPositionId, 1, &lightPosition[0]);

	Controller* controller = renderer->controller;
	glm::vec3 cameraPosition = controller->getCameraPosition();
	glUniform3fv(cameraPositionId, 1, &cameraPosition[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, boneBufferId);
	glBufferData(GL_SHADER_STORAGE_BUFFER, boneMatricesSize, boneMatricesPointer, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, boneBufferId);

	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	glDisable(GL_BLEND);

	glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, (void*)0);

	if (blendEnabled) glEnable(GL_BLEND);

	glDisableVertexAttribArray(vertexPositionId);
	glDisableVertexAttribArray(textureCoordinateId);
	glDisableVertexAttribArray(vertexNormalId);
	glDisableVertexAttribArray(vertexTangentId);
	glDisableVertexAttribArray(vertexBitangentId);
}

void OrderIndependentTransparencyShader::renderShield(glm::mat4 modelMatrix, glm::mat4 shieldMatrix, std::vector<ShieldHit> &hits)
{
	glUseProgram(shieldProgramId);

	renderer->enableVertexAttrib(shieldVertexPositionId, shieldVertexBufferId, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shieldIndexBufferId);

	glm::mat4 viewProjectionMatrix = renderer->getViewProjectionMatrix();
	glm::mat4 modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(shieldModelViewProjectionMatrixId, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(shieldModelMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(shieldShieldMatrixId, 1, GL_FALSE, &shieldMatrix[0][0]);

	glm::vec3 cameraPosition = renderer->getCameraPosition();
	glUniform3fv(shieldCameraPositionId, 1, &cameraPosition[0]);

	GLuint size = sizeof(ShieldHit) * hits.size();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, shieldHitBufferId);
	if (hits.size() > 0)
	{
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, &hits[0], GL_DYNAMIC_COPY);
	}
	else
	{
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, 0, GL_DYNAMIC_COPY);
	}
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shieldHitBufferId);

	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	glDisable(GL_BLEND);

	glDrawElements(GL_TRIANGLES, shieldNumIndices, GL_UNSIGNED_INT, 0);

	if (blendEnabled) glEnable(GL_BLEND);

	glDisableVertexAttribArray(shieldVertexPositionId);
}

void OrderIndependentTransparencyShader::renderFrame()
{
	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	glDisable(GL_BLEND);

	glUseProgram(blenderProgramId);

	glBindImageTexture(3, opaqueFramebufferColour, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	renderer->enableVertexAttrib(blenderVertexPositionId, blenderVertexPositionBufferId, 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(blenderVertexPositionId);

	if (blendEnabled) glEnable(GL_BLEND);
}