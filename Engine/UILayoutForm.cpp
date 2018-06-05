#include "UILayoutForm.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "UIComponent.h"

UILayoutForm::UILayoutForm(UIComponent* component) : UILayout(component)
{
}

void UILayoutForm::layoutComponents()
{
    std::vector<UIComponent*>& components = component->components;
    int numComponents = components.size();
    int numRows = std::ceil(numComponents / rowWidth);

    //First, figure out how wide each of the columns, and how tall each of the rows need to be
    std::vector<float> widestComponents(rowWidth, 0.0f);
    std::vector<float> tallestComponents(numRows, 0.0f);

    for (unsigned int i=0; i<numComponents; i++)
    {
        UIComponent* childComponent = components[i];

        int row = i / rowWidth;
        int column = i % rowWidth;

        glm::vec2 size = childComponent->getSize();

        widestComponents[row] = std::max(size.x, widestComponents[row]);
        tallestComponents[column] = std::max(size.y, tallestComponents[column]);
    }

    for (unsigned int j=0; j<numRows; j++)
    {
        for (unsigned int i=0; i<rowWidth; i++)
        {
            int index = j*rowWidth + i;
            if (index < numComponents)
            {
                UIComponent* childComponent = components[index];
                glm::vec2 size = childComponent->getSize();

                widestComponents[i] = std::max(size.x, widestComponents[i]);
                tallestComponents[j] = std::max(size.y, tallestComponents[j]);
            }
        }
    }

    //Now we know the sizes of the cells, we can start positioning things
    float ypos = externalVerticalBorder;
    for (unsigned int j=0; j<numRows; j++)
    {
        float xpos = externalHorizontalBorder;
        for (unsigned int i=0; i<rowWidth; i++)
        {
            int index = j*rowWidth + i;
            if (index < numComponents)
            {
                UIComponent* childComponent = components[index];
                childComponent->setPosition(xpos, ypos);
                xpos += widestComponents[i] + internalHorizontalBorder;
            }
            ypos += tallestComponents[j] + internalVerticalBorder;
        }
    }

    setSize(xpos + externalHorizontalBorder - internalHorizontalBorder, ypos + externalVerticalBorder - internalVerticalBorder);
}