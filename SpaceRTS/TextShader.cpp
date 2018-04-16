#include "TextShader.h"
#include "Renderer.h"

TextShader::TextShader(Renderer* renderer) : Shader(renderer) {}

void TextShader::initialise()
{
	nextIndex = 0;

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "TextShader.vert" },
		{ GL_FRAGMENT_SHADER, "TextShader.frag" }
	};
	programId = ResourceLoader::loadShaders(shaders, 2);

	vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
	textureCoordinateId = glGetAttribLocation(programId, "vertexTextureCoordinate");
	ratioId = glGetUniformLocation(programId, "screenRatio");
	textureId = glGetUniformLocation(programId, "texture");
	textPositionId = glGetUniformLocation(programId, "textPosition");
	sizeId = glGetUniformLocation(programId, "size");

	glGenBuffers(1, &vertexBufferId);
	glGenBuffers(1, &textureCoordinateBufferId);

	Font* fontPtr = ResourceLoader::loadBFF("Graphics/font.bff");
	if (fontPtr != 0)
	{
		font = *fontPtr;
	}

	for (unsigned int i = 0; i<BUFFER_SIZE; i++)
	{
		verticesBuffer[i] = 0;
		textureCoordinatesBuffer[i] = 0;
	}
}

void TextShader::render(std::vector<glm::vec2> const &vertices, std::vector<glm::vec2> const &textureCoordinates, float x, float y, float size, GLuint texture, float screenRatio)
{
	GLuint verticesSize = vertices.size() * sizeof(glm::vec2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateBufferId);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, &textureCoordinates[0], GL_STATIC_DRAW);

	glUseProgram(programId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureId, 0);

	glEnableVertexAttribArray(vertexPositionId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(vertexPositionId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(textureCoordinateId);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateBufferId);
	glVertexAttribPointer(textureCoordinateId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glUniform1f(ratioId, screenRatio);
	glUniform2f(textPositionId, x, y);
	glUniform1f(sizeId, size);

	GLboolean depthTestEnabled = renderer->getAndSetGlCapability(GL_DEPTH_TEST, false);
	GLboolean blendEnabled = renderer->getAndSetGlCapability(GL_BLEND, true);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	renderer->setGlCapability(GL_DEPTH_TEST, depthTestEnabled);
	renderer->setGlCapability(GL_BLEND, blendEnabled);

	glDisableVertexAttribArray(vertexPositionId);
	glDisableVertexAttribArray(textureCoordinateId);	
}

void TextShader::renderText(const std::string &text, float x, float y, float size, float screenRatio)
{
	std::vector<glm::vec2>* vertices;
	std::vector<glm::vec2>* textureCoordinates;

	std::map<std::string, int>::iterator vecIt = verticesMap.find(text);
	if (vecIt == verticesMap.end())
	{
		vertices = new std::vector<glm::vec2>();
		textureCoordinates = new std::vector<glm::vec2>();

		//Build up panels to display each letter
		float xPos = 0;
		for (unsigned int i = 0; i<text.length(); i++)
		{
			char character = text[i];
			float characterWidth = (float)font.charWidths[character] / font.cellWidth;

			float textureCoordinatesX = (character % font.rowWidth) * font.cellWidthNormalised;
			float textureCoordinatesY = (character / font.rowWidth) * font.cellHeightNormalised;

			float textureWidth = characterWidth / font.rowWidth;

			Geometry::buildPanel(*vertices, glm::vec2(xPos, 0), characterWidth, 1);
			Geometry::buildPanel(*textureCoordinates, glm::vec2(textureCoordinatesX, textureCoordinatesY), textureWidth, font.cellHeightNormalised);

			xPos += characterWidth;
		}

		//Put the heads of the vertex and texture coordinates vectors in the buffers.
		bool needNext = true;
		while (needNext)
		{
			nextIndex++;
			if (nextIndex == BUFFER_SIZE)
			{
				nextIndex = 0;
			}

			if (verticesBuffer[nextIndex] == 0)
			{
				needNext = false;
			}
		}

		verticesBuffer[nextIndex] = vertices;
		textureCoordinatesBuffer[nextIndex] = textureCoordinates;

		verticesMap[text] = nextIndex;
	}
	else
	{
		int index = verticesMap[text];
		vertices = verticesBuffer[index];
		textureCoordinates = textureCoordinatesBuffer[index];
	}

	timerMap[text] = 60;

	render(*vertices, *textureCoordinates, x, y, size, font.textureId, screenRatio);
}