#include "Application.h"

bool Application::Initialise()
{
	if (!glfwInit()) return -1;

	m_window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);

	if (!m_window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL())
	{
		glfwTerminate();
		return -1;
	}

    return true;
}

bool Application::Update()
{
    return true;
}

void Application::Draw()
{
}

void Application::Shutdown()
{
}
