#include "UserInterfaceBarShader.h"
#include "Renderer.h"

UserInterfaceBarShader::UserInterfaceBarShader(Renderer* renderer) : Shader(renderer) {}

void UserInterfaceBarShader::initialise()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "userInterfaceBarShader.vert" },
		{ GL_GEOMETRY_SHADER, "userInterfaceBarShader.geom" },
		{ GL_FRAGMENT_SHADER, "userInterfaceBarShader.frag" }
	};
	programId = ResourceLoader::loadShaders(shaders, 3);

	positionId = glGetAttribLocation(programId, "position");
	dimensionsId = glGetAttribLocation(programId, "dimensions");
	colourId = glGetAttribLocation(programId, "barColour");
	ratioId = glGetUniformLocation(programId, "screenRatio");

	glGenBuffers(1, &positionBufferId);
	glGenBuffers(1, &dimensionsBufferId);
	glGenBuffers(1, &colourBufferId);
}

void UserInterfaceBarShader::render(std::vector<glm::vec2> const &positions, std::vector<glm::vec2> const &dimensions, std::vector<glm::vec4> const &colours, float screenRatio)
{
	GLuint positionsSize = positions.size() * sizeof(glm::vec2);
	GLuint dimensionsSize = dimensions.size() * sizeof(glm::vec2);
	GLuint coloursSize = colours.size() * sizeof(glm::vec4);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
	glBufferData(GL_ARRAY_BUFFER, positionsSize, &positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, dimensionsBufferId);
	glBufferData(GL_ARRAY_BUFFER, dimensionsSize, &dimensions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colourBufferId);
	glBufferData(GL_ARRAY_BUFFER, coloursSize, &colours[0], GL_STATIC_DRAW);

	glUseProgram(programId);

	glEnableVertexAttribArray(positionId);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
	glVertexAttribPointer(positionId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(dimensionsId);
	glBindBuffer(GL_ARRAY_BUFFER, dimensionsBufferId);
	glVertexAttribPointer(dimensionsId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(colourId);
	glBindBuffer(GL_ARRAY_BUFFER, colourBufferId);
	glVertexAttribPointer(colourId, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glUniform1f(ratioId, screenRatio);

	GLboolean depthTestEnabled = renderer->getAndSetGlCapability(GL_DEPTH_TEST, false);
	GLboolean blendEnabled = renderer->getAndSetGlCapability(GL_BLEND, true);

	glDrawArrays(GL_POINTS, 0, positions.size());

	renderer->setGlCapability(GL_DEPTH_TEST, depthTestEnabled);
	renderer->setGlCapability(GL_BLEND, blendEnabled);
}