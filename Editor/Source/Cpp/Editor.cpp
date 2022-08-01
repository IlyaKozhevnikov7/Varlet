#include "Editor.h"

#include "EditorCore.h"
#include "EditViewport.h"
#include "DockSpace.h"
#include "SceneTree.h"
#include "PropertyPanel.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include <iostream>

Varlet::GameModule* EditorData::context;
const Entity* EditorData::selectedEntity;

Editor::Editor(Varlet::GameModule* module)
{
	_panels =
	{
		new DockSpace(),
		new EditViewport(),
		new SceneTree(),
		new PropertyPanel()
	};

	_window = nullptr;
	EditorData::context = module;
	EditorData::selectedEntity = nullptr;
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
		return FAILED_INITIALIZATION;

	InitImGui();

	return EditorData::context->Init();
}

void Editor::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

int32_t Editor::PostInit()
{
	for (auto panel : _panels)
		panel->Init();

	return EditorData::context->PostInit();
}

void Editor::Shutdown()
{
	EditorData::context->Shutdown();

	VARLET_LOG(LevelType::Normal, "Editor shutdown");
}

void Editor::Update()
{
	ImGuiIO io = ImGui::GetIO();

	static bool show_demo_window = true;
	static bool show_another_window = false;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto panel : _panels)
		panel->Update();

	ImGui::ShowDemoWindow(&show_demo_window);

	// Rendering
	ImGui::Render();
	int32_t display_w, display_h;
	glfwGetFramebufferSize(_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.f, 0.f, 0.f, 0.f);
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

	EditorData::context->Update();
}
