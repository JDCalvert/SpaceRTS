#ifndef RENDERER
#define RENDERER

#include <GL\glew.h>
#include <glfw\glfw3.h>
#include <vector>
#include <glm\glm.hpp>

#include "ResourceLoader.h"

class Controller;
class Shader;

/**
* Renderer
* All shader calls should go through the renderer. It maintains a frame buffer, which may be larger
* than the screen itself, which is drawn to by all other shaders. Once everything has been drawn, it
* draws the frame buffer to the screen.
*/
class Renderer
{
public:
    std::vector<glm::vec3> lightPositions;

    //Vector of shaders, so we can tell them when things change
    std::vector<Shader*> shaders;

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
    GLuint screenVertexBufferId;
    std::vector<glm::vec3> screenVertexBufferData;

    //Whole screen shader program
    GLuint screenShaderProgramId;
    GLuint screenShaderVertexId;
    GLuint screenShaderTextureId;

    GLuint screenVertexArrayId;

    Renderer(GLFWwindow* window, Controller* controller);

    void initialise();
    void initialiseScreenShaderProgram();
    void initialiseFramebuffer();

    void initialiseFrame();

    void renderFrame();

    void calculateViewProjectionMatrices();
    void bindArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void bindElementArrayBufferData(GLuint bufferId, int size, void* dataPointer);
    void enableVertexAttrib(GLuint attribId, GLuint bufferId, int attribSize);
    void windowResized(int width, int height);

    int getWidthAntialiasing();
    int getHeightAntialiasing();
    int getPixelCountAntialiasing();

    GLboolean getAndSetGlCapability(GLenum cap, GLboolean enable);
    void setGlCapability(GLenum cap, GLboolean enable);

    glm::mat4 getViewProjectionMatrix();
    glm::vec3 getCameraPosition();
};

#endif