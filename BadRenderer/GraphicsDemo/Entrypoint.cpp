#include "Graphics.h"
#include <iostream>
#include "ShaderProgram.h"
#include <vector>

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "Something went wrong with GLFW set up!\n";
		return -1;
	}

	window = glfwCreateWindow(1280, 720, "Finn's graphics", nullptr, nullptr);

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



	std::vector<float> someFloats
	{
		0, 0, 0,
		0, 1, 0,
		1, 0, 0,
	};

	GLuint vertexBufferID = 0;
	glGenBuffers(1, &vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * someFloats.size(), someFloats.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	

	ShaderProgram testShader("simpleShader.vert", "simpleShader.frag");


	testShader.Use();

	while (!glfwWindowShouldClose(window))
	{
		float rightNow = (float)glfwGetTime();
		float colourIntensity = sin(rightNow) * 0.5f + 0.5f;
		glClearColor(1.0f - colourIntensity, 0.0f, colourIntensity, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		testShader.SetFloatUniform("aspectRatio", 1.7778f);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, someFloats.size() / 3);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}