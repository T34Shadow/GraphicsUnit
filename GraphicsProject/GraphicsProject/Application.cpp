#include "Application.h"

bool Application::Initialise()
{
	if (!glfwInit()) return false;

	m_window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);

	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL())
	{
		glfwTerminate();
		return false;
	}

	m_shader = new ShaderProgram("simpleShader.vert", "simpleShader.frag");

	m_shader->Use();
	glClearColor(0.450f, 0.450f, 0.450f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Gizmo
	unsigned int gridSize = 10000;
	aie::Gizmos::create(gridSize, gridSize, 0.0f, 0.0f);

	//create simple camera transforms 
	m_viewMat = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projectionMat = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//create simple mesh
	//m_quadMesh.InitialiseQuad();
	Mesh::Vertex vertices[6];
	vertices[0].pos = { -0.5f, 0, 0.5f, 1 };
	vertices[1].pos = { 0.5f, 0, 0.5f, 1 };
	vertices[2].pos = { -0.5f, 0, -0.5f, 1 };
				   
	vertices[3].pos = { -0.5f, 0, -0.5f, 1 };
	vertices[4].pos = { 0.5f, 0, 0.5f, 1 };
	vertices[5].pos = { 0.5f, 0, -0.5f, 1 };

	m_quadMesh.Initialise(6, vertices);
	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,01 };

    return true;
}

void Application::Update()
{

}

void Application::Draw()
{
	//clear screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	aie::Gizmos::clear();

	aie::Gizmos::addTransform(glm::mat4(1));

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; i++)
	{
		aie::Gizmos::addLine(glm::vec3(-10.0f + i, 0.0f, 10.0f), glm::vec3(-10.0f + i, 0.0f, -10.0f), i == 10.0f ? white : black);
		aie::Gizmos::addLine(glm::vec3(10.0f, 0.0f, -10.0f + i), glm::vec3(-10.0f, 0.0f, -10.0f + i), i == 10.0f ? white : black);
	}

	aie::Gizmos::draw(m_projectionMat * m_viewMat);

	//bind shader 
	m_shader->Use();

	//bind transform 
	glm::mat4 pvm = m_projectionMat * m_viewMat * m_quadTransform;
	m_shader->SetUniform("ProjectionViewModel", pvm);

	//draw quad
	m_quadMesh.Draw();

	//swapping the buffers // Show the user the screen data. 
	glfwSwapBuffers(m_window);
	//Tell GLFW ti check if anything is going on with the input.
	glfwPollEvents();
	
}

bool Application::Shutdown()
{
	aie::Gizmos::destroy();

	glfwTerminate();
	return true;
}
