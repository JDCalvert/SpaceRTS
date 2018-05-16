#ifndef UI_VERTEX_INFORMATION_H
#define UI_VERTEX_INFORMATION_H

#include <Font.h>
#include <Surface.h>
#include <UIPanel.h>

#include "UIVertexPanel.h"

class UIVertexInformation : public UIPanel
{
public:
    Font* font;
    float textSize;
    float indexWidth;
    float columnWidth;
    float border;

    int startVertex;
    int maxVertices;

    bool showVertices, previousShowVertices;
    bool showTextureCoordinates, previousShowTextureCoordinates;
    bool showNormals, previousShowNormals;

    GLuint blankTexture;

    float xpos;

    UIVertexInformation(Surface* infoSurface);

    void build();
    void preRender();

    bool processMouseScroll(MouseScrollEvent* mouseEvent);

    std::vector<UIVertexPanel*>& getVertexPanels();

private:
    Surface * infoSurface;
    std::vector<UIVertexPanel*> vertexPanels;

    void addHeaderAndSubHeaders(bool shouldAdd, std::string header, char firstSubHeader, int numSubHeaders);
    void addHeader(std::string text, int numColumns);
    void addSubHeader(char subHeader);

    void addVertexPanel(unsigned int i, float ypos);
    void addToggleButton(bool& toggle, float& xpos, float buttonSize, GLuint texture);
};

#endif