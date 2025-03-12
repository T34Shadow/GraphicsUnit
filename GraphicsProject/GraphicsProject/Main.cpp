
#include "Application.h"

#include "Gizmos.h"
#include "ShaderProgram.h"




int main(void)
{
	Application* graphicsAPP;

	graphicsAPP = new Application;
	if (graphicsAPP->Initialise())
	{
		while (graphicsAPP->Update())
		{
			graphicsAPP->Draw();
		}
		graphicsAPP->Shutdown();
	}

	delete graphicsAPP;
	return 0;

	//initialise 

	std::vector<float> someFloats
	{
		0,0,0, 1,0,0,
		0,1,0, 0,1,0,
		1,0,0, 0,0,1,
	};

	GLuint vertexBufferID = 0;
	glGenBuffers(1, &vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * someFloats.size(), someFloats.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	ShaderProgram testShader("simpleShader.vert", "simpleShader.frag");

	testShader.Use();
	glClearColor(0.450f,0.450f,0.450f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	
	//Gizmo
	unsigned int gridSize = 10000;
	aie::Gizmos::create(gridSize, gridSize, 0.0f, 0.0f);
	glm::mat4 viewMat = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMat = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//The main entry point loop.
	while (!glfwWindowShouldClose(window))
	{
		//clear screen.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		aie::Gizmos::clear();

		aie::Gizmos::addTransform(glm::mat4(1));

		glm::vec4 white(1);
		glm::vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; i++)
		{
			aie::Gizmos::addLine(glm::vec3(-10.0f +i, 0.0f, 10.0f), glm::vec3(-10.0f + i, 0.0f, -10.0f), i == 10.0f ? white : black);
			aie::Gizmos::addLine(glm::vec3(10.0f, 0.0f, -10.0f + i), glm::vec3(-10.0f, 0.0f, -10.0f + i), i == 10.0f ? white : black);

		}

		aie::Gizmos::draw(projectionMat * viewMat);

		//testShader.SetFloatUniform("aspectRatio", 1.7778);
		//
		//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(sizeof(float)*0));
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(sizeof(float)*3));
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glDrawArrays(GL_TRIANGLES, 0, someFloats.size() / 6);

		//swapping the buffers // Show the user the screen data. 
		glfwSwapBuffers(window);
		//Tell GLFW ti check if anything is going on with the input.
		glfwPollEvents();
	}

	aie::Gizmos::destroy();

	glfwTerminate();
	return 0;

}