#include "Camera.h"

#include <OpenGLContext.h>

#include <algorithm>
#include <glm\gtc\matrix_transform.hpp>

static const float PI = 4 * atan(1.0);

Camera::Camera(OpenGLContext* glContext)
{
    this->glContext = glContext;

    position = glm::vec3(3.0f, 3.0f, 3.0f);

    horizontalAngle = -3 * PI/4;
    verticalAngle = -PI / 4;

    minDrawDistance = 0.1f;
    maxDrawDistance = 100.0f;
    fieldOfView = 45.0f;

    speed = 3.0f;
    turnSpeed = 2.0f;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(fieldOfView, glContext->getAspectRatio(), minDrawDistance, maxDrawDistance);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}

void Camera::recalculateDirections()
{
    //Recalculate the camera directions
    float forwardX = sin(horizontalAngle) * cos(verticalAngle);
    float forwardY = cos(horizontalAngle) * cos(verticalAngle);
    float forwardZ = sin(verticalAngle);
    forward = glm::vec3(forwardX, forwardY, forwardZ);

    //The direction directly right to the camera (this is always horizontal and so isn't affected by the vertical angle
    float rightX = sin(horizontalAngle + PI / 2);
    float rightY = cos(horizontalAngle + PI / 2);
    float rightZ = 0.0f;
    right = glm::vec3(rightX, rightY, rightZ);

    //The direction directly up from the camera's perspective (this isn't directly up, it's up from the camera's perspective, which is the cross product of the forward and right directions
    up = glm::cross(right, forward);

    //The direction the camera is facing on the horizontal axis. Used to move the camera
    horizontalForward = glm::vec3(sin(horizontalAngle), cos(horizontalAngle), 0.0f);
}

void Camera::rotate(glm::dvec2 deltaMousePosition)
{
    horizontalAngle += deltaMousePosition.x * turnSpeed * glContext->getDeltaTime();
    verticalAngle += deltaMousePosition.y * turnSpeed * glContext->getDeltaTime();
}   

void Camera::move(glm::vec3 direction)
{
    position += direction * speed * (float)glContext->getDeltaTime();
}

void Camera::moveForward()
{
    move(horizontalForward);
}

void Camera::moveBackward()
{
    move(-horizontalForward);
}

void Camera::moveRight()
{
    move(right);
}

void Camera::moveLeft()
{
    move(-right);
}

void Camera::moveUp()
{
    move(up);
}

void Camera::moveDown()
{
    move(-up);
}

void Camera::lookLeft()
{
    horizontalAngle -= PI / 4 * turnSpeed * glContext->getDeltaTime();
}

void Camera::lookRight()
{
    horizontalAngle += PI / 4 * turnSpeed * glContext->getDeltaTime();
}

void Camera::lookUp()
{
    verticalAngle += PI / 4 * turnSpeed * glContext->getDeltaTime();
}

void Camera::lookDown()
{
    verticalAngle -= PI / 4 * turnSpeed * glContext->getDeltaTime();
}

void Camera::ensureAngleWithinRange()
{
    if (horizontalAngle > PI) horizontalAngle -= 2 * PI;
    if (horizontalAngle < -PI) horizontalAngle += 2 * PI;

    verticalAngle = std::min(verticalAngle,  PI / 2);
    verticalAngle = std::max(verticalAngle, -PI / 2);
}
