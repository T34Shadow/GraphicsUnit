
#include "Application.h"

#include "Gizmos.h"
#include "ShaderProgram.h"




int main(void)
{
	Application* graphicsAPP;
	graphicsAPP = new Application;

	if (graphicsAPP->Initialise())
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfw_InitForOpenGL(graphicsAPP->m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		//The main entry point loop.
		while (!glfwWindowShouldClose(graphicsAPP->m_window))
		{
			if ((glfwGetKey(graphicsAPP->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
			{
				break;
			}
			float delta = glfwGetTime();

			graphicsAPP->Update(delta);
			graphicsAPP->Draw();
			
		}
		if (graphicsAPP->Shutdown())
		{
			return 0;
		}
	}
}