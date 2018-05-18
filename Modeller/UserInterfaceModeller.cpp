#include "UserInterfaceModeller.h"
#include "UIVertexPanel.h"

UserInterfaceModeller::UserInterfaceModeller(Surface* infoSurface)
{
    this->infoSurface = infoSurface;
}

UserInterfaceModeller* UserInterfaceModeller::getInstance()
{
    return (UserInterfaceModeller*)UserInterface::getInstance();
}

void UserInterfaceModeller::build()
{
    renderOptions = new UIRenderOptions();
    renderOptions->build();
    addComponent(renderOptions);

    vertexInformation = new UIVertexInformation(infoSurface);
    vertexInformation->build();
    addComponent(vertexInformation);

    triangleInformation = new UITriangleInformation(infoSurface);
    triangleInformation->build();
    addComponent(triangleInformation);

    boneInformation = new UIBoneInformation(infoSurface);
    boneInformation->build();
    addComponent(boneInformation);

    saveLoadPanel = new UISaveLoadPanel();
    saveLoadPanel->build();
    addComponent(saveLoadPanel);
}

void UserInterfaceModeller::loadSurface(const char* fileName)
{
    infoSurface->loadFromFile(fileName);

    vertexInformation->build();
    triangleInformation->build();
}

void UserInterfaceModeller::saveSurface(const char* fileName)
{
    infoSurface->writeToFile(fileName);
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