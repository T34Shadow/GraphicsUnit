
#define GLFW_INCLUDE_NONE
#include "glfw3.h"
#include "glad.h"
#include "glm.hpp"

#include "Utilities.h"
#include "math.h"


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
	
	//The main entry point loop.
	while (!glfwWindowShouldClose(window))
	{
		//clear screen.
		glClear(GL_COLOR_BUFFER_BIT);

		//do rendering code here.
		float b = 1;
		float y = (sin(b * glfwGetTime())) * 0.5f + 0.5;
		
		glm::vec3 mix = glm::mix(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), y);

		glClearColor(mix.r, mix.g, mix.b, 1.0f);

		//swapping the buffers // Show the user the screen data. 
		glfwSwapBuffers(window);

		//Tell GLFW ti check if anything is going on with the input.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}