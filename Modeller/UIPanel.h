#ifndef UI_PANEL_H
#define UI_PANEL_H

#include <glfw\glfw3.h>
#include <glm\glm.hpp>

class UIPanel
{
public:
    glm::vec2 position;
    glm::vec2 size;

    GLuint texture;
};

#endif