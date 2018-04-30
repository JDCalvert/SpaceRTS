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

    void update();

    glm::vec2 getMousePosition();
};

#endif