#ifndef CONTROLLER
#define CONTROLLER

#include <GL\glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class Camera;
class OpenGLContext;

class Controller
{
private:

    Camera* camera;
    OpenGLContext* glContext;

    float mouseSpeed;
    float previousMouseX, previousMouseY;

    double time;

public:
    Controller(Camera* camera, OpenGLContext* glContext);

    float deltaTime;

    void mouseButtonCallback(int button, int action, int mods);
    void scrollCallback(double xOffset, double yOffset);

    void update(GLFWwindow* window);
    void cleanUpFrame();
    void calculateCameraPosition(GLFWwindow* window);
    void ensureAnglesWithinRange();

    glm::vec2 getMousePosition();

    glm::vec3 getCameraPosition();
    glm::vec3 getCameraForward();
    glm::vec3 getCameraRight();
    glm::vec3 getCameraUp();
};

#endif