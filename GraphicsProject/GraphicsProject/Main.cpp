
#include "Graphics.h"
#include "glm.hpp"
#include "ShaderProgram.h"

#include <iostream>
#include <vector>
#include <math.h>


int main(void)
{
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) 
	{
		glfwTerminate();
		return -1;
	}

	//initialise 

	std::vector<float> someFloats
	{
		0,0,0,
		0,1,0,
		1,0,0,
	};

	GLuint vertexBufferID = 0;
	glGenBuffers(1, &vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * someFloats.size(), someFloats.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	ShaderProgram testShader("simpleShader.vert", "simpleShader.frag");

	testShader.Use();

	//The main entry point loop.
	while (!glfwWindowShouldClose(window))
	{

		//do rendering code here.
		float b = 1;
		float y = (sin(b * glfwGetTime())) * 0.5f + 0.5;
		
		glm::vec3 mix = glm::mix(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), y);

		glClearColor(mix.r, mix.g, mix.b, 1.0f);
		//clear screen.
		glClear(GL_COLOR_BUFFER_BIT);

		testShader.SetFloatUniform("aspectRatio", 1.7778);


		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, someFloats.size() / 3);


		//swapping the buffers // Show the user the screen data. 
		glfwSwapBuffers(window);
		//Tell GLFW ti check if anything is going on with the input.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}