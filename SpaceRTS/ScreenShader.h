#ifndef SCREEN_SHADER_H
#define SCREEN_SHADER_H

#include "Shader.h"

class ScreenShader : public Shader
{
	public:
	GLuint vertexPositionId;
	GLuint screenShaderTextureId;

	void initialise()
	{

	}
};

#endif