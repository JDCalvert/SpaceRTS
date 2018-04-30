#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>

class OpenGLContext;

class Camera
{
public:
    Camera();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

    void rotate(glm::dvec2 deltaMousePosition);
    void recalculateDirections();

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    void lookLeft();
    void lookRight();
    void lookUp();
    void lookDown();

private:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;

    glm::vec3 horizontalForward;

    float horizontalAngle;
    float verticalAngle;
    
    float minDrawDistance;
    float maxDrawDistance;
    float fieldOfView;

    float speed;
    float turnSpeed;
    float mouseSensitivity;

    void move(glm::vec3 direction);
    void look(float& angle, float rate);
    void ensureAngleWithinRange();
};

#endif