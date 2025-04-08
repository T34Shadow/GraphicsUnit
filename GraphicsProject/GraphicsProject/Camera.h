#pragma once
#include "Graphics.h"

class Camera
{
public:

	glm::vec3 position;
	float pitch = 0.0f; //looking up and down
	float yaw = 0.0f; //looking left and right
	//float roll = 0.0f; //skewing the camera, let's just not do this

	float aspectRatio = 1280.0f / 720.0f;

	float fov = glm::radians(60.0f);

	void Update(float delta, GLFWwindow* _window);

	glm::mat4 GetVPMatrix() const;

	glm::vec3 GetForwardVec() const;
	glm::vec3 GetRightVec() const;

	void PrintPos() const;
};