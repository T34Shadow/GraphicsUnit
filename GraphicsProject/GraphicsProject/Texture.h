#pragma once

#include "Graphics.h"
#include <string>
class ShaderProgram;

class Texture
{
	GLuint texID = 0;

public:

	Texture() = default;
	~Texture();
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	void LoadFromFile(std::string filename);
	void Bind(std::string name, ShaderProgram* shader, int textureUnit = 0) const;

};