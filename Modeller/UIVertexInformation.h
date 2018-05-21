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
    float buttonSize;

    bool showVertices, previousShowVertices;
    bool showTextureCoordinates, previousShowTextureCoordinates;
    bool showNormals, previousShowNormals;
    bool showBones, previousShowBones;

    float xpos;

    UIVertexInformation(Surface* infoSurface);

    void build() override;
    void preRender();

    std::vector<UIVertexPanel*>& getVertexPanels();

    void actionPerformed(UIComponent* component);
    
protected:
    UIComponent* addPanel(unsigned int i) override;
    int getNumItemsTotal() override;

private:
    std::vector<UIVertexPanel*> vertexPanels;

    void addHeaderAndSubHeaders(bool shouldAdd, std::string header, char firstSubHeader, int numSubHeaders);
    void addHeader(std::string text, int numColumns);
    
    void addSubHeader(char subHeader, float width);
    void addSubHeader(std::string subHeader, float width);

    void addToggleButton(bool& toggle, std::string text);
};

#endif