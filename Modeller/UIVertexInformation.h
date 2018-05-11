#ifndef UI_VERTEX_INFORMATION_H
#define UI_VERTEX_INFORMATION_H

#include <Surface.h>

#include "UIPanel.h"

struct Font;

class UIVertexInformation : public UIPanel
{
public:
    void build(Surface* surface);

    void preRender();

private:
    Surface* infoSurface;

    Font* font;
    float textSize;
    float indexWidth;
    float columnWidth;
    float border;

    bool showVertices, previousShowVertices;
    bool showTextureCoordinates, previousShowTextureCoordinates;
    bool showNormals, previousShowNormals;

    void rebuildPanels();

    void addRowVec2(glm::vec2& row, float& xpos, float ypos, bool shouldAddHeader, std::string header, char firstSubHeader, GLuint texture);
    void addRowVec3(glm::vec3& row, float& xpos, float ypos, bool shouldAddHeader, std::string header, char firstSubHeader, GLuint texture);

    void addHeader(std::string text, float xPosition, int numColumns);
    void addSubHeader(std::string text, float xPosition);
    void addNumber(float& number, glm::vec2 position, GLuint background);
    void addIndexLabel(unsigned int& number, glm::vec2 position);

    void addToggleButton(bool& toggle, float& xpos, float buttonSize, GLuint texture);
};

#endif