#pragma once

#include "Graphics.h"
#include "Vertex.h"

#include <vector>
#include <iostream>

class ShaderProgram;

class Mesh
{
private:

	unsigned int triCount = 0;
	unsigned int vertCount = 0;
	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int indexBufferObject;

	glm::vec3 Ka; // ambient colour of the surface;
	glm::vec3 Kd; // diffuse colour of the surface;
	glm::vec3 Ks; // specular colour of the surface;
	float specularPower; // tightness of specualr highlights;

public:

	Mesh() : triCount(0), vertexArrayObject(0), vertexBufferObject(0), indexBufferObject(0){}

	virtual ~Mesh();
	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void InitialiseFromFile(std::string fileName);

	void ApplyMat(ShaderProgram* shader);
	void LoadMat(const char* fileName);

	void Equip();
	void Unequip();
	virtual void Draw();


	//TEMP
public:
	void InitialiseQuad();


};