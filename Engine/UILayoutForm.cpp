#include "UILayoutForm.h"

#include <algorithm>

#include "UIComponent.h"

UILayoutForm::UILayoutForm(UIComponent* component, unsigned int groupSize) :
    groupSize(groupSize),
    UILayout(component)
{
}

void UILayoutForm::layoutComponents()
{
    int groupComponent = getComponentForGroup();
    int indexComponent = getComponentForIndex();

    std::vector<UIComponent*>& components = component->components;
    unsigned int numComponents = components.size();
    unsigned int numGroups = std::ceil((float)numComponents / groupSize);

    //The form will end up being a grid, we just need to know how large each of the cells are
    std::vector<float> largestInGroup(numGroups, 0.0f);
    std::vector<float> largestInIndex(groupSize, 0.0f);

    for (unsigned int i=0; i<numComponents; i++)
    {
        UIComponent* childComponent = components[i];

        int group = i / groupSize;
        int index = i % groupSize;

        glm::vec2 size = childComponent->getSize();

        largestInGroup[group] = std::max(size[indexComponent], largestInGroup[group]);
        largestInIndex[index] = std::max(size[groupComponent], largestInIndex[index]);
    }

    //Now we know the sizes of the cells, we can start positioning things
    float groupPos;
    float indexPos = externalBorder[indexComponent];
    for (unsigned int j = 0; j<numGroups; j++)
    {
        groupPos = externalBorder[groupComponent];
        for (unsigned int i = 0; i<groupSize; i++)
        {
            int index = j * groupSize + i;
            if (index < numComponents)
            {
                UIComponent* childComponent = components[index];

                glm::vec2 position;
                position[groupComponent] = groupPos;
                position[indexComponent] = indexPos;

                childComponent->setPosition(position);
                groupPos += largestInIndex[i] + internalBorder[groupComponent];
            }
        }
        indexPos += largestInGroup[j] + internalBorder[indexComponent];
    }

    glm::vec2 size;
    size[groupComponent] = groupPos + externalBorder[groupComponent] - internalBorder[groupComponent];
    size[indexComponent] = indexPos + externalBorder[indexComponent] - internalBorder[indexComponent];
    component->setSize(size);
}

void UILayoutForm::stretchComponents()
{
}