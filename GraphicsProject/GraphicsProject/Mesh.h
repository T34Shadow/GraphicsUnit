#pragma once

#include "Graphics.h"

class Mesh
{
public:

	Mesh() : triCount(0), vertexArrayObject(0),vertexBufferObject(0),indexBufferObject(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 pos;
		glm::vec4 normal;
		glm::vec2 textureCoord;
	};

	void InitialiseQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	virtual void Draw();

protected:

	unsigned int triCount;
	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int indexBufferObject;

};