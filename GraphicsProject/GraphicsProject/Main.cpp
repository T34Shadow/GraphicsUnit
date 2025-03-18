
#include "Application.h"

#include "Gizmos.h"
#include "ShaderProgram.h"




int main(void)
{
	Application* graphicsAPP;
	
	graphicsAPP = new Application;
	if (graphicsAPP->Initialise())
	{
		//The main entry point loop.
		while (!glfwWindowShouldClose(graphicsAPP->m_window))
		{
			graphicsAPP->Update();
			graphicsAPP->Draw();
			//swapping the buffers // Show the user the screen data. 
			glfwSwapBuffers(graphicsAPP->m_window);
			//Tell GLFW ti check if anything is going on with the input.
			glfwPollEvents();
		}
	}

	if (graphicsAPP->Shutdown())
	{
		return 0;
	}



}