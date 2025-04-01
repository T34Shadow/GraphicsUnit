#pragma once

#include "Graphics.h"

class Mesh;
class Texture;
class ShaderProgram;

class MeshInstance
{
public:
	glm::vec3 position{ 0 };
	glm::vec3 rotation{ 0 };
	glm::vec3 scale{ 1 };

	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	ShaderProgram* shader = nullptr;

	void Draw(glm::mat4 viewpMatrix) const;
};