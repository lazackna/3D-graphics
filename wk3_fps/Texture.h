#pragma once

#include <gl/glew.h>
#include <string>

class Texture
{
	GLuint id;
public:
	std::string name;
public:
	Texture(const std::string &fileName);
	void bind();
	
};