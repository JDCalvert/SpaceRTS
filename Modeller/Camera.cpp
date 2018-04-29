#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

static const double PI = 4 * atan(1.0);

Camera::Camera()
{
    position = glm::vec3(3.0f, 3.0f, 3.0f);

    horizontalAngle = -3 * PI/4;
    verticalAngle = -PI / 4;
    fieldOfView = 45.0f;

    speed = 3.0f;
    turnSpeed = 2.0f;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}

