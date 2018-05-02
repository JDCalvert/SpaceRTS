#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "UIComponent.h"

class UILabel : public UIComponent
{
public:
    void setText(std::string text);

protected:
    void processMouseEvent(MouseEvent* mouseEvent);

private:
    std::string text;

    std::vector<glm::vec2> vertexPositions;
    std::vector<glm::vec2> vertexTexCoords;
    std::vector<unsigned int> indices;
};

#endif
