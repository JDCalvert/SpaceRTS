#ifndef UI_VERTEX_PANEL_H
#define UI_VERTEX_PANEL_H

#include "UIPanel.h"
#include "UIActionListener.h"

class UIVertexInformation;

class UIVertexPanel : public UIPanel, UIActionListener
{
public:
    glm::vec3& vertexPosition;
    glm::vec2& vertexTextureCoordinate;
    glm::vec3& vertexNormal;
    glm::vec4& vertexBoneIndices;

    UIVertexPanel(UIVertexInformation* parent, Surface* infoSurface, unsigned int index);

    void preRender() override;
    void buildPanel();

    bool isHighlighted();

    void actionPerformed(UIComponent* component) override;

private:
    UIVertexInformation* parent;

    unsigned int index;

    float xpos;

    bool highlighted;
    GLuint onMap, offMap;

    void addIndexLabel();
    void addRowVec2(glm::vec2& row);
    void addRowVec3(glm::vec3& row);
    void addNumber(float& number, int numDigits, float width);
    void addBones();
};

#endif