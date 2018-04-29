#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}

