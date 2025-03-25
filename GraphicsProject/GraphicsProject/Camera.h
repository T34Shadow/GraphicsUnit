#pragma once
#include "Graphics.h"

class Camera
{
public:

	glm::mat4 GetVeiwMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);
	glm::vec3 GetFowardVector(float theta, float phi);

	void Update(float delta);

private:
	float m_theta;
	float m_phi;
	glm::vec3 m_pos;
};