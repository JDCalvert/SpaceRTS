#ifndef RENDERER
#define RENDERER

#include <GL\glew.h>
#include <glfw\glfw3.h>
#include <vector>
#include <glm\glm.hpp>

#include "ResourceLoader.h"
#include "UserInterfaceShader.h"
#include "UserInterfaceBarShader.h"
#include "TextShader.h"

class World;
class WorldObject;
class Controller;
class Surface;
class UserInterfacePanel;
class OrderIndependentTransparencyShader;

class Renderer
{
	public:
	std::vector<glm::vec3> lightPositions;

	//Shaders
	OrderIndependentTransparencyShader* orderIndependentTransparencyShader;

	//Window and matrices
	GLFWwindow* window;
	Controller* controller;
	int width, height;
	int antiAliasingFactor;
	float fieldOfView;
	float screenRatio;
	float minDrawDistance;
	float maxDrawDistance;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	
	//Whole screen framebuffer
	GLuint frameBufferId;
	GLuint depthRenderbufferId;
	GLuint renderedTexture;
	GLuint screenVertexArrayId;
	GLuint screenVertexBufferId;
	std::vector<glm::vec3> screenVertexBufferData;

	//Whole screen shader program
	GLuint screenShaderProgramId;
	GLuint screenShaderVertexId;
	GLuint screenShaderTextureId;

	//Shaders
	UserInterfaceShader* userInterfaceShader = new UserInterfaceShader(this);
	UserInterfaceBarShader* userInterfaceBarShader;
	TextShader* textShader;
	
	Renderer(GLFWwindow* window, Controller* controller);

	void initialise();
	void initialiseScreenShaderProgram();
	void initialiseFramebuffer();
	
	void initialiseFrame();

	void renderFrame();

	void renderWorld(World* world);
	void renderText(const char* text, float x, float y, float size);
	void renderUserInterfacePanel(UserInterfacePanel* userInterfacePanel);
	void renderBars(std::vector<glm::vec2> const &vertices, std::vector<glm::vec2> const &dimensions, std::vector<glm::vec4> const &colours);

	void calculateViewProjectionMatrices();
	void bindArrayBufferData(GLuint bufferId, int size, void* dataPointer);
	void bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer);
	void enableVertexAttrib(GLuint attribId, GLuint bufferId, int attribSize);
	void windowResized(int width, int height);
	void buildPanel(std::vector<glm::vec2> &vertices, glm::vec2 topLeft, float width, float height);

	int getWidthAntialiasing();
	int getHeightAntialiasing();
	int getPixelCountAntialiasing();

	GLboolean getAndSetGlCapability(GLenum cap, GLboolean enable);
	void setGlCapability(GLenum cap, GLboolean enable);

	glm::mat4 getViewProjectionMatrix();
	glm::vec3 getCameraPosition();
};

#endif