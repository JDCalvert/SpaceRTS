#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>

class Camera
{
public:
    glm::mat4 getViewMatrix();

private:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
};

#endif