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
	mainCamera.position = glm::vec3(0, 10, 10);
	mainCamera.pitch = glm::radians(-30.0f);

	//Gizmo
	unsigned int gridSize = 10000;
	aie::Gizmos::create(gridSize, gridSize, 0.0f, 0.0f);

	////create simple camera transforms 
	//m_viewMat = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_projectionMat = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//Assest loading;

	//SoulSpear
	
	soulSpearMesh->InitialiseFromFile("soulspear.obj");

	//load materials
	soulSpearMesh->LoadMat("soulspear.mtl");

	//add textures
	Texture* spearDiffuse = new Texture();
	Texture* spearNormal = new Texture();
	Texture* spearSpecular = new Texture();

	spearDiffuse->LoadFromFile("soulspear_diffuse.tga");
	spearNormal->LoadFromFile("soulspear_normal.tga");
	spearSpecular->LoadFromFile("soulspear_specular.tga");

	//Suzanne
	Mesh* suzanneMesh = new Mesh();
	suzanneMesh->InitialiseFromFile("Suzanne.fbx");
	//add textures
	Texture* suzanneDiffuse = new Texture();
	suzanneDiffuse->LoadFromFile("SuzanneTestTex.png");

	//Initialise scene objcets
	MeshInstance spear;
	spear.mesh = soulSpearMesh;
	spear.texture = spearDiffuse;
	spear.shader = m_shader;
	spear.position = glm::vec3(-10, 0, 0);

	MeshInstance spear02;
	spear02.mesh = soulSpearMesh;
	spear02.texture = spearDiffuse;
	spear02.shader = m_shader;
	spear02.position = glm::vec3(10, 0, 0);

	MeshInstance spear03;
	spear03.mesh = soulSpearMesh;
	spear03.texture = spearDiffuse;
	spear03.shader = m_shader;
	spear03.position = glm::vec3(0, 0, 0);

	MeshInstance monkey;
	monkey.mesh = suzanneMesh;
	monkey.texture = suzanneDiffuse;
	monkey.shader = m_shader;
	monkey.position = glm::vec3(5, 0, 0);

	MeshInstance monkey02;
	monkey02.mesh = suzanneMesh;
	monkey02.texture = suzanneDiffuse;
	monkey02.shader = m_shader;
	monkey02.position = glm::vec3(-5, 0, 0);

	objects.push_back(spear);	
	objects.push_back(spear02);	
	objects.push_back(spear03);	
	objects.push_back(monkey);
	objects.push_back(monkey02);

	//Directional light
	m_directionalLight.direction = glm::vec3(1, 1, 0);
	m_directionalLight.rotationSpeed = 0.5;
	m_directionalLight.colour = { 1,1,1 };

	//Ambient light
	m_ambientLight = { 0.25, 0.25, 0.25 };
    return true;
}

void Application::Update(float delta)
{	
	mainCamera.Update(delta, m_window);
	m_directionalLight.direction = glm::normalize(glm::vec3(glm::cos(delta * m_directionalLight.rotationSpeed), glm::sin(delta * m_directionalLight.rotationSpeed), 0));
}

void Application::Draw()
{
	//clear screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Initialise ImGui of a new frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

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


	//bind shader 
	m_shader->Use();

	//bind light 
	m_shader->SetUniform("AmbientColour", m_ambientLight);
	m_shader->SetUniform("LightColour", m_directionalLight.colour);
	m_shader->SetUniform("LightDirection", m_directionalLight.direction);

	//bind mats 
	soulSpearMesh->ApplyMat(m_shader);
	
	//bind transform 
	m_shader->SetUniform("ProjectionViewModel", vpMat);

	//bind transform for lighting
	m_shader->SetUniform("ModelMatrix", m_identityMatrix);
	
	//draw objects
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Draw(vpMat);
	}
	aie::Gizmos::draw(vpMat);

	//Draw the ImGui frames.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//swapping the buffers // Show the user the screen data. 
	glfwSwapBuffers(m_window);
	//Tell GLFW to check if anything is going on with the input.
	glfwPollEvents();
	
}

bool Application::Shutdown()
{
	aie::Gizmos::destroy();

	glfwTerminate();
	return true;
}
