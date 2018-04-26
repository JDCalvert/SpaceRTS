#ifndef CONTROLLER
#define CONTROLLER

#include <GL\glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class Controller
{
private:
    glm::vec3 cameraPosition;
    glm::vec3 cameraForward;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    float horizontalAngle;
    float verticalAngle;
    float fieldOfView;

    float speed;
    float mouseSpeed;
    float turnSpeed;

    float previousMouseX, previousMouseY;

    double time;

    bool mouseButtonPressed(GLFWwindow* window, int key);

public:
    Controller();

    float deltaTime;

    void mouseButtonCallback(int button, int action, int mods);
    void scrollCallback(double xOffset, double yOffset);

    bool keyPressed(GLFWwindow* window, int key);

    void update(GLFWwindow* window);
    void cleanUpFrame();
    void calculateCameraPosition(GLFWwindow* window);
    void ensureAnglesWithinRange();

    bool leftMouseButtonPressed;
    bool leftMouseButtonDown;

    bool middleMouseButtonPressed;
    bool middleMouseButtonDown;

    bool rightMouseButtonPressed;
    bool rightMouseButtonDown;

    glm::vec2 getMousePosition();

    glm::vec3 getCameraPosition();
    glm::vec3 getCameraForward();
    glm::vec3 getCameraRight();
    glm::vec3 getCameraUp();
};

#endif