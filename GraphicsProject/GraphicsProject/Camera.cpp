#include "Camera.h"

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
    return glm::vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));
}

void Camera::Update(float delta)
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    glm::vec3 forward = GetFowardVector(thetaR, phiR);
}

