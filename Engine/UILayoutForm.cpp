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
    largestInGroup.resize(numGroups, 0.0f);
    largestInIndex.resize(groupSize, 0.0f);

    for (unsigned int i=0; i<numComponents; i++)
    {
        UIComponent* childComponent = components[i];

        int group = i / groupSize;
        int index = i % groupSize;

        childComponent->layoutComponents();

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

    glm::vec2 size = calculateComponentsSize() + externalBorder * 2.0f;
    component->setSize(size);
}

void UILayoutForm::stretchComponents()
{
    unsigned int numComponents = component->components.size();
    int numGroups = calculateNumGroups();
    int groupComponent = getComponentForGroup();
    int indexComponent = getComponentForIndex();

    glm::vec2 idealComponentsSize = component->getSize() - externalBorder * 2.0f;
    glm::vec2 actualComponentsSize = calculateComponentsSize();

    glm::vec2 sizeChange = idealComponentsSize - actualComponentsSize;

    //Now increase the size of the last group and index to match
    largestInGroup[numGroups-1] += sizeChange[indexComponent];
    largestInIndex[groupSize-1] += sizeChange[groupComponent];
    
    //Now, we can go through the components and stretch them to fill their cell
    for (unsigned int i=0; i<numComponents; i++)
    {
        UIComponent* childComponent = component->components[i];

        int group = i / groupSize;
        int index = i % groupSize;

        glm::vec2 size;
        size[groupComponent] = largestInIndex[index];
        size[indexComponent] = largestInGroup[group];

        childComponent->setSize(size);
        childComponent->stretchComponents();
    }
}

int UILayoutForm::calculateNumGroups()
{
    unsigned int numComponents = component->components.size();
    return std::ceil((float)numComponents / groupSize);
}

glm::vec2 UILayoutForm::calculateComponentsSize()
{
    int numGroups = calculateNumGroups();
    int groupComponent = getComponentForGroup();
    int indexComponent = getComponentForIndex();

    glm::vec2 actualComponentsSize(0.0f);

    for (unsigned int i = 0; i<groupSize; i++)
    {
        actualComponentsSize[groupComponent] += largestInIndex[i];
    }

    for (unsigned int i = 0; i<numGroups; i++)
    {
        actualComponentsSize[indexComponent] += largestInGroup[i];
    }

    actualComponentsSize[groupComponent] += internalBorder[groupComponent] * (groupSize - 1);
    actualComponentsSize[indexComponent] += internalBorder[indexComponent] * (numGroups - 1);

    return actualComponentsSize;
}