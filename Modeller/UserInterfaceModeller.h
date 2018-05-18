#ifndef USER_INTERFACE_MODELLER_H
#define USER_INTERFACE_MODELLER_H

#include <UserInterface.h>

#include "UIRenderOptions.h"
#include "UIVertexInformation.h"
#include "UITriangleInformation.h"
#include "UISaveLoadPanel.h"

class UserInterfaceModeller : public UserInterface
{
public:
    UIRenderOptions * renderOptions;
    UIVertexInformation* vertexInformation;
    UITriangleInformation* triangleInformation;
    UISaveLoadPanel* saveLoadPanel;

    UserInterfaceModeller(Surface* infoSurface);

    static UserInterfaceModeller* getInstance();

    void build();

    void loadSurface(const char* fileName);
    void saveSurface(const char* fileName);

    std::vector<glm::vec3> getHighlightVertices();
    std::vector<unsigned int> getHighlightIndices();

private:
    Surface* infoSurface;
};

#endif