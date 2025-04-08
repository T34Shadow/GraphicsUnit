#pragma once

#include "Gizmos.h"
#include "ShaderProgram.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Mesh.h"
#include "Graphics.h"
#include "Camera.h"
#include <iostream>
#include <vector>
#include <math.h>

class Application
{
public:

	Application() = default;
	
	//do rule of three

	bool Initialise();
	void Update(float delta);
	void Draw();
	bool Shutdown();

	GLFWwindow* m_window;
	Camera* m_camera;
	
	//Gizmo
	unsigned int gridSize = 10000;
	Camera mainCamera;

	//matrix
	glm::mat4 m_viewMat;
	glm::mat4 m_projectionMat;

	//mesh
	ShaderProgram* m_shader;
	std::vector<Mesh*> objects;
	glm::mat4 m_quadTransform;


	//Buffers
	GLuint vertexBufferID = 0;
	
	struct Light
	{
		glm::vec3 direction;
	};

	//lights
	Light m_light;


};