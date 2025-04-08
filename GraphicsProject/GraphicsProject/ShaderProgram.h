#pragma once

#include "Graphics.h"
#include <string>

class ShaderProgram
{
private:
	
	GLuint shaderProgram = 0;

	GLuint fragmentShader = 0;
	GLuint vertexShader = 0;

public:
	ShaderProgram() {}
	ShaderProgram(std::string vertexFilename, std::string fragmentFilename);

	~ShaderProgram();

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Use();

	//void SetFloatUniform(std::string varName, float value);
	void SetUniform(std::string varName, glm::mat4& value);
	void SetUniform(std::string varName, float& value);
	void SetUniform(std::string varName, int& value);
};