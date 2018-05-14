#include "UserInterfaceModeller.h"
#include "UIVertexPanel.h"

#include "PointShader.h"

UserInterfaceModeller* UserInterfaceModeller::getInstance()
{
    return (UserInterfaceModeller*)UserInterface::getInstance();
}

void UserInterfaceModeller::build(Surface* infoSurface)
{
    renderOptions = new UIRenderOptions();
    renderOptions->build();
    addComponent(renderOptions);

    vertexInformation = new UIVertexInformation();
    vertexInformation->build(infoSurface);
    addComponent(vertexInformation);
}

std::vector<glm::vec3> UserInterfaceModeller::getHighlightPoints()
{
    std::vector<glm::vec3> highlights;
    std::vector<UIVertexPanel*>& vertexPanels = vertexInformation->getVertexPanels();
    for (auto i = vertexPanels.begin(); i != vertexPanels.end(); i++)
    {
        UIVertexPanel* vertexPanel = *i;
        if (vertexPanel->isHighlighted()) highlights.push_back(vertexPanel->vertexPosition);
    }

    return highlights;
}