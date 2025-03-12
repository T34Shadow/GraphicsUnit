#pragma once

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
	bool Update();
	void Draw();
	void Shutdown();

protected:

	glm::mat4 m_viewMat;
	glm::mat4 m_projectionMat;

	GLFWwindow* m_window;

};