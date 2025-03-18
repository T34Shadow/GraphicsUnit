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
	glClearColor(0.450f, 0.450f, 0.450f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Gizmo
	unsigned int gridSize = 10000;
	aie::Gizmos::create(gridSize, gridSize, 0.0f, 0.0f);
	m_viewMat = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projectionMat = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);


    return true;
}

void Application::Update()
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
}

void Application::Draw()
{
}

bool Application::Shutdown()
{
	aie::Gizmos::destroy();

	glfwTerminate();
	return true;
}
