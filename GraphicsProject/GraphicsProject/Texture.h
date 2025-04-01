#pragma once

#include "Graphics.h"
#include <string>
class ShaderPrgram;

class Texture
{
	GLuint texID = 0;

public:

	Texture() = default;
	~Texture();
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	void LoadFromFile(std::string filename);
	void Bind(std::string name, ShaderPrgram* shader, int textureUnit = 0) const;

};