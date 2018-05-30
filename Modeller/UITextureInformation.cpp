#include "UITextureInformation.h"

#include <algorithm>

#include <OpenGLContext.h>
#include <Texture.h>

#include "UserInterfaceModeller.h"
#include "BlankShader.h"

UITextureInformation::UITextureInformation(Surface* infoSurface) : UIPanel()
{
    this->infoSurface = infoSurface;

    showSpecular = false;
    showNormal = false;

    buttonSize = 0.02f;
    textureSize = 0.2f;
    height = 0.0f;

    surface.diffuseMap = Texture::getTexture("Blank");
}

void UITextureInformation::build()
{
    textureCoordinateRenderer = Renderer::initialiseRenderer(new TextureRenderer(0.2f), OpenGLContext::currentContext());
    textureCoordinateRenderer->setClearColour(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    
    GLuint textureCoordinateId = textureCoordinateRenderer->getTexture();

    xpos = textureSize - 0.032f;

    addToggleButton(showSpecularButton, showSpecular, "S");
    addToggleButton(showNormalButton, showNormal, "N");

    addTexturePanel(diffusePanel, infoSurface->diffuseMap, textureCoordinateId, "Diffuse");
    addTexturePanel(specularPanel, infoSurface->specularMap, textureCoordinateId, "Specular");
    addTexturePanel(normalPanel, infoSurface->normalMap, textureCoordinateId, "Normal");

    setSize(xpos, height);

    rebuildPanel();

    UserInterfaceModeller::getInstance()->textureInformation->rebuildImportTextureCoordinates();
}

void UITextureInformation::rebuildPanel()
{
    components.clear();
    addComponent(showSpecularButton);
    addComponent(showNormalButton);

    xpos = 0.01f;

    replaceTexturePanel(diffusePanel);
    if (showSpecular) replaceTexturePanel(specularPanel);
    if (showNormal) replaceTexturePanel(normalPanel);

    setSize(xpos, height);
}

void UITextureInformation::renderTextureCoordinates()
{
    textureCoordinateRenderer->initialiseFrame();

    std::vector<unsigned int> highlightVertices = UserInterfaceModeller::getInstance()->getHighlightVertexIndices();
    std::vector<unsigned int> highlightIndices = UserInterfaceModeller::getInstance()->getHighlightTriangleIndices();

    glm::vec4 textureCoordinateColour(1.0f, 0.0f, 0.0f, 1.0f);

    BlankShader* blankShader = static_cast<BlankShader*>(Shader::getShader("Blank"));
    blankShader->renderTextureCoordinateVertices(infoSurface, highlightVertices, textureCoordinateColour);
    blankShader->renderTextureCoordinateLines(infoSurface, highlightIndices, textureCoordinateColour);

    UIImportPanel* importPanel = UserInterfaceModeller::getInstance()->importPanel;
    if (importPanel->hover)
    {        
        blankShader->renderLines(importVertices, importIndices, glm::mat4(), textureCoordinateColour);
    }
}

void UITextureInformation::rebuildImportTextureCoordinates()
{
    UIImportPanel* importPanel = UserInterfaceModeller::getInstance()->importPanel;
    glm::vec2 topLeft = importPanel->textureCoordinatesTopLeft * 2.0f - glm::vec2(1.0f, 1.0f);
    glm::vec2 bottomRight = importPanel->textureCoordinatesBottomRight * 2.0f - glm::vec2(1.0f, 1.0f);

    importVertices =
    {
        glm::vec3(topLeft, 0.0f),
        glm::vec3(topLeft.x, bottomRight.y, 0.0f),
        glm::vec3(bottomRight, 0.0f),
        glm::vec3(bottomRight.x, topLeft.y, 0.0f),
    };

    importIndices =
    {
        0, 1,
        1, 2,
        2, 3,
        3, 0
    };
}

void UITextureInformation::preRender()
{
    if (showSpecular != previousShowSpecular
     || showNormal != previousShowNormal)
    {
        rebuildPanel();

        previousShowSpecular = showSpecular;
        previousShowNormal = showNormal;
    }

    float xpos = UserInterfaceModeller::getInstance()->triangleInformation->getSize().x + 0.02f;;
    float ypos = 0.99f - height;
    setPosition(xpos, ypos);
}

void UITextureInformation::addTexturePanel(UITexturePanel*& texturePanel, GLuint& textureId, GLuint textureCoordinateId, std::string textureName)
{
    texturePanel = new UITexturePanel(this, textureId, textureCoordinateId, textureName);
    texturePanel->build();
    height = std::max(height, texturePanel->getSize().y + 0.02f);
}

void UITextureInformation::addToggleButton(UIToggleButton*& toggleButton, bool& toggle, std::string buttonText)
{
    toggleButton = new UIToggleButton(toggle);
    toggleButton->setPosition(xpos, 0.01f);
    toggleButton->setSize(buttonSize, buttonSize);
    toggleButton->setText(buttonText, buttonSize, Font::getFont("Calibri"), CENTRE);

    xpos += buttonSize + 0.002f;
}

void UITextureInformation::replaceTexturePanel(UITexturePanel* texturePanel)
{
    texturePanel->setPosition(xpos, 0.01f);
    addComponent(texturePanel);
    xpos += textureSize + 0.01f;
}