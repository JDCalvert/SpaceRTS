#include "UserInterfaceShader.h"
#include "Renderer.h"

UserInterfaceShader::UserInterfaceShader(Renderer* renderer) : Shader(renderer) {}

void UserInterfaceShader::initialise()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "userInterfaceShader.vert" },
		{ GL_FRAGMENT_SHADER, "userInterfaceShader.frag" }
	};
	programId = ResourceLoader::loadShaders(shaders, 2);

	vertexPositionId = glGetAttribLocation(programId, "vertexPosition");
	textureCoordinateId = glGetAttribLocation(programId, "vertexTextureCoordinate");
	ratioId = glGetUniformLocation(programId, "screenRatio");
	textureId = glGetUniformLocation(programId, "texture");

	glGenBuffers(1, &vertexBufferId);
	glGenBuffers(1, &textureCoordinateBufferId);
}

void UserInterfaceShader::render(std::vector<glm::vec2> const &vertices, std::vector<glm::vec2> const &textureCoordinates, GLuint texture, float screenRatio)
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

	GLboolean depthEnabled = renderer->getAndSetGlCapability(GL_DEPTH_TEST, false);
	GLboolean blendEnabled = renderer->getAndSetGlCapability(GL_BLEND, true);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	renderer->setGlCapability(GL_DEPTH_TEST, depthEnabled);
	renderer->setGlCapability(GL_BLEND, blendEnabled);

	glDisableVertexAttribArray(vertexPositionId);
	glDisableVertexAttribArray(textureCoordinateId);
}