#ifndef USER_INTERFACE_BAR_SHADER_H
#define USER_INTERFACE_BAR_SHADER_H

#include "Shader.h"

class UserInterfaceBarShader : Shader
{
private:
	
	GLuint positionId;
	GLuint dimensionsId;
	GLuint colourId;
	GLuint ratioId;

	GLuint positionBufferId;
	GLuint dimensionsBufferId;
	GLuint colourBufferId;

public:

	UserInterfaceBarShader(Renderer* renderer);

	void initialise();
	void render(std::vector<glm::vec2> const &positions, std::vector<glm::vec2> const &dimensions, std::vector<glm::vec4> const &colours, float screenRatio);
};

#endif