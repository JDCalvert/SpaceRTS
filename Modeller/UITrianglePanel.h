#ifndef UI_TRIANGLE_PANEL_H
#define UI_TRIANGLE_PANEL_H

#include <UIPanel.h>

class UITriangleInformation;

class UITrianglePanel : public UIPanel
{
public:
    unsigned int& index1, index2, index3;

    UITrianglePanel(UITriangleInformation* parent, Surface* infoSurface, unsigned int firstIndex);

    void preRender();
    void buildPanel();

    bool isHighlighted();
    void addIndices(std::vector<unsigned int>& indices);

private:
    UITriangleInformation* parent;

    float xpos;

    bool highlighted;
    GLuint onMap, offMap;

    void addNumber(unsigned int& index);
};

#endif