#ifndef USER_INTERFACE_MODELLER_H
#define USER_INTERFACE_MODELLER_H

#include <UserInterface.h>

#include "UIRenderOptions.h"
#include "UIVertexInformation.h"
#include "UITriangleInformation.h"
#include "UIBoneInformation.h"
#include "UISaveLoadPanel.h"

class UserInterfaceModeller : public UserInterface
{
public:
    UIRenderOptions * renderOptions;
    UIVertexInformation* vertexInformation;
    UITriangleInformation* triangleInformation;
    UIBoneInformation* boneInformation;
    UISaveLoadPanel* saveLoadPanel;

    UserInterfaceModeller(Surface* infoSurface);

    static UserInterfaceModeller* getInstance();

    void build();

    void loadSurface(const char* fileName);
    void saveSurface(const char* fileName);

    std::vector<glm::vec3> getHighlightVertices();
    std::vector<unsigned int> getHighlightIndices();

    void recalculateVertexPositions();

    void newVertex();
    void newBone();

private:
    Surface* infoSurface;
};

#endif