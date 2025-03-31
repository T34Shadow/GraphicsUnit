#include "Camera.h"
#include <iostream>

glm::mat4 Camera::GetVeiwMatrix()
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    return glm::lookAt(m_pos, m_pos + GetFowardVector(thetaR,phiR), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.0f);
}

glm::vec3 Camera::GetFowardVector(float theta, float phi)
{
    float thetaR = glm::radians(theta);
    float phiR = glm::radians(phi);
    return glm::vec3(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
}

glm::vec3 Camera::GetRightVector(float theta)
{
    float thetaR = glm::radians(theta);
    return glm::vec3(0,0,1);
}

void Camera::Update(float delta, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		std::cout << "move FORWARD " << std::endl;
		(m_pos += GetFowardVector(m_theta,m_phi) * delta) * m_speed;
		PrintPos();
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		std::cout << "move RIGHT " << std::endl;
		(m_pos += GetRightVector(m_theta) * delta)* m_speed;
		PrintPos();
	}

	if (glfwGetKey(window, GLFW_KEY_A))
	{
		std::cout << "move LEFT " << std::endl;
		(m_pos -= GetRightVector(m_theta) * delta)* m_speed;
		PrintPos();
	}

	if (glfwGetKey(window, GLFW_KEY_S))
	{
		std::cout << "move BACKWARDS " << std::endl;
		(m_pos -= GetFowardVector(m_theta, m_phi) * delta)* m_speed;
		PrintPos();
	}
	if (glfwGetKey(window, GLFW_KEY_X))
	{
		std::cout << "move UPWARDS " << std::endl;
		(m_pos += GetUpVector() * delta)* m_speed; 
		PrintPos();
	}
	if (glfwGetKey(window, GLFW_KEY_Z))
	{
		std::cout << "move DOWNWARDS " << std::endl;
		(m_pos -= GetUpVector() * delta)* m_speed;
		PrintPos();
	}

}

void Camera::PrintPos()
{
	std::cout << "X - " << m_pos.x << " : Y - " << m_pos.y << " : Z - " << m_pos.z << std::endl;
}

