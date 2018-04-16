#ifndef USER_INTERFACE_SHADER_H
#define USER_INTERFACE_SHADER_H

#include "Shader.h"

class UserInterfaceShader : public Shader
{
private:
	GLuint vertexPositionId;
	GLuint textureCoordinateId;
	GLuint ratioId;
	GLuint textureId;

	GLuint vertexBufferId;
	GLuint textureCoordinateBufferId;

public:

	UserInterfaceShader(Renderer* renderer);
	void initialise();
	void render(std::vector<glm::vec2> const &vertices, std::vector<glm::vec2> const &textureCoordinates, GLuint texture, float screenRatio);
};

#endif