#include "Application.h"

bool Application::Initialise()
{
	if (!glfwInit()) return false;

	m_window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
	m_camera = new Camera();
	
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

	//Scene stuff (maybe this goes in a scene class later?)
	
	glfwSetWindowUserPointer(m_window, &mainCamera);

	//Initialise camera
	mainCamera.position = glm::vec3(2, 2, 2);
	mainCamera.pitch = glm::radians(-30.0f);

	//Gizmo
	unsigned int gridSize = 10000;
	aie::Gizmos::create(gridSize, gridSize, 0.0f, 0.0f);

	//create simple camera transforms 
	m_viewMat = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projectionMat = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//create simple mesh
	//m_quadMesh.InitialiseQuad();
	Vertex vertices[6];
	vertices[0].pos = { -0.5f, 0, 0.5f, 1 };
	vertices[1].pos = { 0.5f, 0, 0.5f, 1 };
	vertices[2].pos = { -0.5f, 0, -0.5f, 1 };
				   
	vertices[3].pos = { -0.5f, 0, -0.5f, 1 };
	vertices[4].pos = { 0.5f, 0, 0.5f, 1 };
	vertices[5].pos = { 0.5f, 0, -0.5f, 1 };

	//Assest loading;
	Mesh soulSpearMesh;
	soulSpearMesh.Initialise("soulspear");
	//add textures

	//Initialise scene objcets
	MeshInstance spear;
	spear.mesh = &soulSpearMesh;
	//spear.texture = nullptr;
	spear.shader = m_shader;

	spear.position = glm::vec3(0, 0, 0);
	objects.push_back(spear);

    return true;
}

void Application::Update(float delta)
{	
	mainCamera.Update(delta, m_window);
	m_light.direction = glm::normalize(glm::vec3(glm::cos(delta * 2), glm::sin(delta * 2), 0));
}

void Application::Draw()
{
	//Initialise ImGui of a new frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//clear screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//create ImGui objects.
	
	//ImGui::ShowDemoWindow();
	//Controls Window.
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Camera Controls");

	ImGui::SetWindowPos(ImVec2(10, 10));
	ImGui::SetWindowSize(ImVec2(160, 202));

	ImGui::Text("Forwards: W");
	ImGui::Text("Backwards: S");
	ImGui::Text("Move Right: D");
	ImGui::Text("Move Left: A");
	ImGui::Text("Yaw Right: L");
	ImGui::Text("Yaw Left: J");
	ImGui::Text("Pitch Up: I");
	ImGui::Text("Pitch Down: K");
	ImGui::Text("Upwards: SPACEBAR");
	ImGui::Text("Downwards: LEFT ALT");

	ImGui::End();
	
	glm::mat4 vpMat = mainCamera.GetVPMatrix();

	aie::Gizmos::clear();

	aie::Gizmos::addTransform(glm::mat4(1));

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; i++)
	{
		aie::Gizmos::addLine(glm::vec3(-10.0f + i, 0.0f, 10.0f), glm::vec3(-10.0f + i, 0.0f, -10.0f), i == 10.0f ? white : black);
		aie::Gizmos::addLine(glm::vec3(10.0f, 0.0f, -10.0f + i), glm::vec3(-10.0f, 0.0f, -10.0f + i), i == 10.0f ? white : black);
	}

	aie::Gizmos::draw(vpMat);

	//bind shader 
	m_shader->Use();

	//bind light 
	

	//bind transform 
	glm::mat4 pvm = vpMat;
	m_shader->SetUniform("ProjectionViewModel", pvm);

	//bind transform for lighting
	m_shader->SetUniform("ModelMatrix", m_quadTransform);

	//draw objects
	for (MeshInstance& object : objects)
	{
		object.Draw(vpMat);
	}

	//Draw the ImGui frames.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
