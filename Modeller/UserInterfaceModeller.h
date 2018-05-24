#ifndef USER_INTERFACE_MODELLER_H
#define USER_INTERFACE_MODELLER_H

#include <UserInterface.h>

#include "UIRenderOptions.h"
#include "UIVertexInformation.h"
#include "UITriangleInformation.h"
#include "UIBoneInformation.h"
#include "UITextureInformation.h"
#include "UISaveLoadPanel.h"

class UserInterfaceModeller : public UserInterface
{
public:
    UIRenderOptions * renderOptions;
    UIVertexInformation* vertexInformation;
    UITriangleInformation* triangleInformation;
    UIBoneInformation* boneInformation;
    UITextureInformation* textureInformation;
    UISaveLoadPanel* saveLoadPanel;

    UserInterfaceModeller(Surface* infoSurface);

    static UserInterfaceModeller* getInstance();

    void build();
    void rebuildInformation();

    void loadSurface(const char* fileName);
    void saveSurface(const char* fileName);
    void importSurface(const char* fileName);

    std::vector<unsigned int> getHighlightVertexIndices();
    std::vector<unsigned int> getHighlightTriangleIndices();

    void recalculateVertexPositions();

    void newVertex();
    void newBone();

    void updateVertexPosition(int index, glm::vec3 newVertexPosition);

    void removeVertices(int index);
    void removeVertex(unsigned int index);

private:
    Surface* infoSurface;
};

#endif