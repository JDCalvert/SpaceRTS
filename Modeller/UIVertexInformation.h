#ifndef UI_VERTEX_INFORMATION_H
#define UI_VERTEX_INFORMATION_H

#include "UIPanel.h"

class UIVertexInformation : public UIPanel
{
public:
    void build(std::vector<glm::vec3>& vertices);

};

#endif