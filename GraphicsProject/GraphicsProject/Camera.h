#pragma once
#include "Graphics.h"

class Camera
{
public:

	Camera() = default;

	glm::mat4 GetVeiwMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	glm::vec3 GetFowardVector(float theta, float phi);
	glm::vec3 GetRightVector(float theta);
	glm::vec3 GetUpVector() { return glm::vec3(0, 1, 0); }

	void Update(float delta, GLFWwindow* window);

private:

	void PrintPos();

	float m_theta;
	float m_phi;
	float m_speed = 0.1f;
	glm::vec3 m_pos;
};