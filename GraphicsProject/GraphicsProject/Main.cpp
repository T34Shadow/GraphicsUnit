
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
			float delta = glfwGetTime();

			graphicsAPP->Update(delta);
			graphicsAPP->Draw();
		}
	}

	if (graphicsAPP->Shutdown())
	{
		return 0;
	}



}