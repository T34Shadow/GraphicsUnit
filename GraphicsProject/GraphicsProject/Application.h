#pragma once

#include "Gizmos.h"
#include "ShaderProgram.h"

#include "Graphics.h"
#include <iostream>
#include <vector>
#include <math.h>

class Application
{
public:

	Application() {};
	
	//do rule of three

	bool Initialise();
	void Update();
	void Draw();
	bool Shutdown();

	GLFWwindow* m_window;
	
	//Gizmo
	unsigned int gridSize = 10000;
	glm::mat4 m_viewMat;
	glm::mat4 m_projectionMat;

	//Buffers
	GLuint vertexBufferID = 0;
	



};