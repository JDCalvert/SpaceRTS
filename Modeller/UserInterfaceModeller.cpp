#include "UserInterfaceModeller.h"
#include "UIVertexPanel.h"

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

    triangleInformation = new UITriangleInformation();
    triangleInformation->build(infoSurface);
    addComponent(triangleInformation);

    saveLoadPanel = new UISaveLoadPanel();
    saveLoadPanel->build();
    addComponent(saveLoadPanel);
}

std::vector<glm::vec3> UserInterfaceModeller::getHighlightVertices()
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

std::vector<unsigned int> UserInterfaceModeller::getHighlightIndices()
{
    std::vector<unsigned int> highlights;
    std::vector<UITrianglePanel*>& trianglePanels = triangleInformation->getTrianglePanels();
    for (auto i = trianglePanels.begin(); i != trianglePanels.end(); i++)
    {
        UITrianglePanel* trianglePanel = *i;
        if (trianglePanel->isHighlighted()) trianglePanel->addIndices(highlights);
    }

    return highlights;
}