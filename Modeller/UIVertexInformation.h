#ifndef UI_VERTEX_INFORMATION_H
#define UI_VERTEX_INFORMATION_H

#include <Font.h>

#include "UIInformation.h"
#include "UIVertexPanel.h"

class UIVertexInformation : public UIInformation
{
public:
    float indexWidth;
    float columnWidth;

    bool showVertices, previousShowVertices;
    bool showTextureCoordinates, previousShowTextureCoordinates;
    bool showNormals, previousShowNormals;

    float xpos;

    UIVertexInformation(Surface* infoSurface);

    void build() override;
    void preRender();

    std::vector<UIVertexPanel*>& getVertexPanels();
    
protected:
    UIComponent* addPanel(unsigned int i) override;
    int getNumItemsTotal() override;

private:
    std::vector<UIVertexPanel*> vertexPanels;

    void addHeaderAndSubHeaders(bool shouldAdd, std::string header, char firstSubHeader, int numSubHeaders);
    void addHeader(std::string text, int numColumns);
    void addSubHeader(char subHeader);

    void addToggleButton(bool& toggle, float& xpos, float buttonSize, GLuint texture);
};

#endif