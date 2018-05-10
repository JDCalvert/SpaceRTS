#ifndef UI_VERTEX_INFORMATION_H
#define UI_VERTEX_INFORMATION_H

#include "UIPanel.h"

struct Font;

class UIVertexInformation : public UIPanel
{
public:
    void build(std::vector<glm::vec3>& vertices);

private:
    Font* font;
    float textSize;
    float columnWidth;

    void addHeader(std::string text, float xPosition);
    void addNumber(float& number, glm::vec2 position, GLuint background);
};

#endif