#include "Editor.h"
#include "EditorCamera.h"
#include "Core/VarletAPI.h"
#include "Rendering/Texture.h"

#include "VarletFramework.h"

#include "glad/glad.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include <iostream>

Editor::Editor(Varlet::GameModule* module)
{
	_window = nullptr;
	_context = module;
	_mainCamera = nullptr;
}

int32_t Editor::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return FAILED_INITIALIZATION;
	}

	_window = VarletAPI::GetContextWindow<GLFWwindow>();

	if (_window == nullptr)
	{
		return FAILED_INITIALIZATION;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return _context->Init();
}

int32_t Editor::PostInit()
{
	auto scene = _context->GetCurrentScene();
	auto camera = scene->CreateEntity();

	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>()->SetActive(true);
	_mainCamera = camera->AddComponent<EditorCamera>();

	_mainCamera->InternalStart();

	return _context->PostInit();
}

void Editor::Shutdown()
{
	_context->Shutdown();

	std::cout << "Editor shutdown" << std::endl;
}

void Editor::Update()
{
	_mainCamera->InternalUpdate();

	ImGuiIO io = ImGui::GetIO();

	// Our state
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		DrawCameraInfo();
		DrawDockSpace();
		DrawViewPort();
	}

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	//_context->Update();
}

void Editor::DrawCameraInfo()
{
	const auto rotation = _mainCamera->GetTransform()->GetRotation();
	const auto eulerAngles = _mainCamera->GetTransform()->GetEulerAngles();

	ImGui::Begin("Editor Camera");

	ImGui::Text("X: %f, Y: %f, Z: %f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("W: %f", rotation.w);

	ImGui::Separator();

	ImGui::Text("Pitch: %f", glm::degrees(eulerAngles.x));
	ImGui::Text("Yaw: %f", glm::degrees(eulerAngles.y));
	ImGui::Text("Roll: %f", glm::degrees(eulerAngles.z));

	ImGui::End();
}

void Editor::DrawDockSpace() const
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar 
		| ImGuiWindowFlags_NoDocking 
		| ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoCollapse 
		| ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus 
		| ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", nullptr, window_flags);

	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				VARLET_LOG(LevelType::Normal, "New Scene Pressed");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void Editor::DrawViewPort() const
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

	ImGui::SetNextWindowSize(ImVec2(640.f, 480.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	ImGui::Begin("Viewport", nullptr, windowFlags);

	auto texture = _mainCamera->GetRendereTexture();
	texture->Activate(0);

	ImGui::Image(reinterpret_cast<ImTextureID>(texture->GetId()), ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::PopStyleVar();

	static ImVec2 lastSize = ImGui::GetWindowSize();
	
	if (lastSize.x != ImGui::GetWindowSize().x || lastSize.y != ImGui::GetWindowSize().y)
		_mainCamera->OnResize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

	lastSize = ImGui::GetWindowSize();

	ImGui::End();
}
