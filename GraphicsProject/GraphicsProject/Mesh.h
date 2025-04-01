#pragma once

#include "Graphics.h"
#include "Vertex.h"

#include <vector>

class Mesh
{
private:

	unsigned int triCount = 0;
	unsigned int vertCount = 0;
	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int indexBufferObject;

public:

	Mesh() : triCount(0), vertexArrayObject(0), vertexBufferObject(0), indexBufferObject(0){}

	virtual ~Mesh();
	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	void Initialise(const char* fileName);
	void LoadFromFile(std::string filename);

	void Equip();
	void Unequip();
	virtual void Draw();

	//TEMP
public:

	void InitialiseQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);


};