#include "Application.h"

bool Application::Initialise()
{
	if (!glfwInit()) return false;

	m_window = glfwCreateWindow(1280, 720, "TestingPlatform", nullptr, nullptr);
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

	//create simple camera transforms 
	//m_viewMat = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_projectionMat = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//Assest loading;

	//SoulSpear
	
	soulSpearMesh->InitialiseFromFile("soulspear.obj");


	//load materials
	soulSpearMesh->LoadMat("soulspear.mtl");
	//add textures
	Texture* spearDiffuse = new Texture(0);
	Texture* spearNormal = new Texture(1);
	Texture* spearSpecular = new Texture(2);

	spearDiffuse->LoadFromFile("soulspear_diffuse.tga");
	spearNormal->LoadFromFile("soulspear_normal.tga");
	spearSpecular->LoadFromFile("soulspear_specular.tga");

	////Suzanne
	//Mesh* suzanneMesh = new Mesh();
	//suzanneMesh->InitialiseFromFile("Suzanne.fbx");
	////add textures
	//Texture* suzanneDiffuse = new Texture(0);
	//suzanneDiffuse->LoadFromFile("SuzanneTestTex.png");

	//Initialise scene objcets
	MeshInstance spear;
	spear.mesh = soulSpearMesh;
	spear.diffuseTex = spearDiffuse;
	spear.normalTex = spearNormal;
	spear.specularTex = spearSpecular;
	spear.shader = m_shader;
	spear.position = glm::vec3(-10, 0, 0);

	MeshInstance spear02;
	spear02.mesh = soulSpearMesh;
	spear02.diffuseTex = spearDiffuse;
	spear02.normalTex = spearNormal;
	spear02.specularTex = spearSpecular;
	spear02.shader = m_shader;
	spear02.position = glm::vec3(10, 0, 0);

	MeshInstance spear03;
	spear03.mesh = soulSpearMesh;
	spear03.rotation = glm::vec3(0, 1, 0);
	spear03.diffuseTex = spearDiffuse;
	spear03.normalTex = spearNormal;
	spear03.specularTex = spearSpecular;
	spear03.shader = m_shader;
	spear03.position = glm::vec3(0, 0, 0);
	spear03.rotation = glm::vec3(0, 1, 0);

	//MeshInstance monkey;
	//monkey.mesh = suzanneMesh;
	//monkey.diffuseTex = suzanneDiffuse;
	//monkey.shader = m_shader;
	//monkey.position = glm::vec3(5, 0, 0);
	//monkey.rotation = glm::vec3(180, 0, 0);
	//
	//MeshInstance monkey02;
	//monkey02.mesh = suzanneMesh;
	//monkey02.diffuseTex = suzanneDiffuse;
	//monkey02.shader = m_shader;
	//monkey02.position = glm::vec3(-5, 0, 0);
	//monkey02.rotation = glm::vec3(180, 0, 0);

	objects.push_back(spear);	
	objects.push_back(spear02);	
	objects.push_back(spear03);	
	//objects.push_back(monkey);
	//objects.push_back(monkey02);

	//Directional light
	lightDirection = glm::vec3(0, 0, 1);
	m_directionalLight.direction = lightDirection;
	m_directionalLight.rotationSpeed = 1;
	m_directionalLight.colour = { 0,1,0 };

	lightDirection02 = glm::vec3(1, 0, 0);
	m_directionalLight02.direction = lightDirection02;
	m_directionalLight02.rotationSpeed = 2;
	m_directionalLight02.colour = { 0,0,0 };


    return true;
}

void Application::Update(float delta)
{	
	mainCamera.Update(delta, m_window);
	if (glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		mainCamera.PrintPos();
	}
	if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS)
	{
		m_directionalLight02.colour = { 1,0,0 };
	}
	if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_PRESS)
	{
		m_directionalLight02.colour = { 0,0,0 };
	}
	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_directionalLight.colour = { 0,1,0 };
	}
	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_directionalLight.colour = { 0,0,0 };
	}
	if (glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS)
	{
		m_directionalLight.direction = glm::normalize(glm::vec3(glm::cos(delta * m_directionalLight.rotationSpeed), glm::sin(delta * m_directionalLight.rotationSpeed), 0));
	}
	if (glfwGetKey(m_window, GLFW_KEY_P) == GLFW_PRESS)
	{
		m_directionalLight02.direction = glm::normalize(glm::vec3(glm::cos(delta * -m_directionalLight02.rotationSpeed), glm::sin(delta * -m_directionalLight02.rotationSpeed), 0));
	}
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
	ImGui::SetWindowSize(ImVec2(160, 300));

	ImGui::Text("Forwards: W");
	ImGui::Text("Backwards: S");
	ImGui::Text("Move Right: D");
	ImGui::Text("Move Left: A");
	ImGui::Text("Yaw Right: L");
	ImGui::Text("Yaw Left: J");
	ImGui::Text("Pitch Up: I");
	ImGui::Text("Pitch Down: K");
	ImGui::Text("Upwards: SPACEBAR");
	ImGui::Text("Light 1 on: 1");
	ImGui::Text("Light 1 off: 2");
	ImGui::Text("Light 2 on: 3");
	ImGui::Text("Light 2 off: 4");
	ImGui::Text("Move Light 1: O");
	ImGui::Text("Move Light 2: P");

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
	m_shader->SetUniform("LightColour", m_directionalLight.colour);
	m_shader->SetUniform("LightColour02", m_directionalLight02.colour);
	m_shader->SetUniform("LightDirection", m_directionalLight.direction);
	m_shader->SetUniform("LightDirection02", m_directionalLight02.direction);

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
