#ifndef UI_TRIANGLE_PANEL_H
#define UI_TRIANGLE_PANEL_H

#include <UIPanel.h>
#include <UIActionListener.h>
#include <UIButton.h>

class UITriangleInformation;

class UITrianglePanel : public UIPanel, public UIActionListener
{
public:
    unsigned int index;
    unsigned int& index1;
    unsigned int& index2;
    unsigned int& index3;

    UITrianglePanel(UITriangleInformation* parent, Surface* infoSurface, unsigned int firstIndex);

    void preRender();
    void buildPanel();

    bool isHighlighted();
    void addIndices(std::vector<unsigned int>& indices);

    void actionPerformed(UIComponent* component);

private:
    UITriangleInformation* parent;

    UIButton* removeButton;

    float xpos;

    bool highlighted;
    GLuint onMap, offMap;

    void addNumber(unsigned int& index);
    void addRemoveButton();
};

#endif