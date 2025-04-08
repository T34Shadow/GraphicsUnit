#include "MeshInstance.h"

#include "ShaderProgram.h"
#include "Texture.h"
#include "Mesh.h"


#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"

void MeshInstance::Draw(glm::mat4 viewpMatrix) const
{
	mesh->Equip();
	texture->Bind("albedoMap", shader);

	glm::mat4 modelMat = glm::scale(glm::mat4(1), scale);
	modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0));
	modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));

	modelMat = glm::translate(modelMat, position);

	glm::mat4 mvpMat = viewpMatrix * modelMat;

	shader->SetUniform("mvpMat", mvpMat);
	shader->SetUniform("modelMat", modelMat);

	mesh->Draw();

	mesh->Unequip();
	
}
