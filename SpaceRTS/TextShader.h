#ifndef TEXT_SHADER_H
#define TEXT_SHADER_H

#include <map>

#include "Shader.h"
#include "Geometry.h"

class TextShader : Shader
{
public:
	GLuint vertexPositionId;
	GLuint textureCoordinateId;
	GLuint ratioId;
	GLuint textureId;
	GLuint textPositionId;
	GLuint sizeId;

	GLuint vertexBufferId;
	GLuint textureCoordinateBufferId;

	Font font;

	std::map<std::string, int> verticesMap;
	std::map<std::string, int> timerMap;
	
	static const int BUFFER_SIZE = 1000000;
	std::vector<glm::vec2>* verticesBuffer[BUFFER_SIZE];
	std::vector<glm::vec2>* textureCoordinatesBuffer[BUFFER_SIZE];

	int nextIndex;

	TextShader(Renderer* renderer);

	void initialise();
	void render(std::vector<glm::vec2> const &vertices, std::vector<glm::vec2> const &textureCoordinates, float x, float y, float size, GLuint texture, float screenRatio);
	void renderText(const std::string &text, float x, float y, float size, float screenRatio);
};

#endif