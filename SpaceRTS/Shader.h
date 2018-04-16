#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

#include "ResourceLoader.h"

class Renderer;

class Shader
{
public:
	Shader(Renderer* renderer);

protected:
	GLuint programId;
	Renderer* renderer;

	virtual void initialise() = 0;
};

#endif