#include "Camera.h"

#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"

#include <iostream>

void Camera::Update(float delta, GLFWwindow* _window)
{
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		yaw += 0.01f;
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		yaw -= 0.01f;
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		pitch += 0.01f;
		if (pitch > glm::radians(80.0f))
		{
			pitch = glm::radians(80.0f);
		}
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		pitch -= 0.01f;
		if (pitch < glm::radians(-80.0f))
		{
			pitch = glm::radians(-80.0f);
		}
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += GetForwardVec() * 0.1f;
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= GetForwardVec() * 0.1f;
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position += glm::vec3(0,1,0) * 0.1f;
		PrintPos();
	}
	if (glfwGetKey(_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		position -= glm::vec3(0, 1, 0) * 0.1f;
		PrintPos();
	}
}

glm::mat4 Camera::GetVPMatrix() const
{
	glm::mat4 projectionMat = glm::perspective(fov, aspectRatio, 0.3f, 20.0f);

	glm::mat4 viewMat(1);
	viewMat = glm::translate(viewMat, position);

	viewMat = glm::rotate(viewMat, yaw, glm::vec3(0, 1, 0));
	viewMat = glm::rotate(viewMat, pitch, glm::vec3(1, 0, 0));


	viewMat = glm::inverse(viewMat);

	return projectionMat * viewMat;

}

glm::vec3 Camera::GetForwardVec() const
{
	glm::vec4 forward(0.0f, 0.0f, -1.0f, 0.0f);
	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, yaw, glm::vec3(0, 1, 0));
	rotationMat = glm::rotate(rotationMat, pitch, glm::vec3(1, 0, 0));

	forward = rotationMat * forward;

	return glm::vec3(forward.x, forward.y, forward.z);
}

void Camera::PrintPos()
{
	std::cout << "X - " << position.x << " : Y - " << position.y << " : Z - " << position.z << std::endl;
}
