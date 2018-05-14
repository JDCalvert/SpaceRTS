#ifndef USER_INTERFACE_MODELLER_H
#define USER_INTERFACE_MODELLER_H

#include <UserInterface.h>

#include "UIRenderOptions.h"
#include "UIVertexInformation.h"

class UserInterfaceModeller : public UserInterface
{
public:
    UIRenderOptions* renderOptions;
    UIVertexInformation* vertexInformation;

    static UserInterfaceModeller* getInstance();

    void build(Surface* infoSurface);

    std::vector<glm::vec3> getHighlightPoints();
};

#endif