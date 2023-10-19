#include "Utilities/ImGuiPanelUtility.h"
#include "Panels/ImGuiPanel.h"
#include "Screen.h"
#include "Panels/DockSpace.h"

#include "glad/glad.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/ImGuizmo.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

void ImGuiPanelUtility::Initialize()
{
	BaseClass::Initialize();

	InitializeImGui();

	_defaultPropertyPanel = std::make_shared<DefaultImGuiPropertyPanel>();
}

void ImGuiPanelUtility::Update()
{
	const auto mainWindow = Screen::GetNative();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	UpdatePanels();

	//ImGui::ShowDemoWindow();

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(mainWindow, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup_current_context);

	glfwSwapBuffers(mainWindow);
}

void ImGuiPanelUtility::OnRegisteredPanelType(const Varlet::Core::Type* type)
{
	if (type->flags.Has(Varlet::Core::TypeMetaFlags::Abstract) || IsImGuiPanelType(type) == false)
		return;

	if (type->IsA(ImGuiContextPanelBase))
	{
		_contextPanelTypes.push_back(type);
		return;
	}

	if (type->IsA(InitializationAssetPanelBase))
	{
		_initializationPanelTypes.push_back(type);
		return;
	}

	auto panel = std::bit_cast<int8_t*>(type->CreateInstance());
	assert(panel);

	EditorPanel* newPanel = (EditorPanel*)CastMemory(type, EditorPanel::GetTypeStatic(), panel);
	assert(newPanel);

	_actions.push_back({ newPanel, PanelActionType::Add });
}

void ImGuiPanelUtility::OnUnregisteredPanelType(const Varlet::Core::Type* type)
{
}

void ImGuiPanelUtility::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	BaseClass::Shutdown();
}

void ImGuiPanelUtility::DisplayProperty(const Varlet::Core::Type* type, const char* name, void* context, bool isPointer)
{
	assert(type);

	for (size_t i = 1; i < _propertyPanels.size(); i++)
	{
		const auto panel = _propertyPanels[i];

		if (type->CheckIsA(panel->GetContextType()))
		{
			panel->Display(name, context, isPointer);
			return;
		}
	}

	_defaultPropertyPanel->contextType = type;
	_defaultPropertyPanel->Display(name, context, isPointer);
}

const Varlet::Core::Type* ImGuiPanelUtility::GetContextPanelFor(const Varlet::Core::Type* contextType)
{
	static const uint32_t panelTemplteId = Varlet::Core::Type::GetTemplateId(NAME_OF(ImGuiContextPanel));

	for (auto& panelType : _contextPanelTypes)
	{
		const Varlet::Core::Type* type = panelType;

		for (auto& info : type->parentInfos)
		{
			if (info.type->module != nullptr)
				continue;

			const auto templateType = dynamic_cast<const Varlet::Core::TemplateType*>(info.type);
			assert(templateType);

			if (templateType->templateId != panelTemplteId)
				continue;

			if (contextType->CheckIsA(templateType->parameterList[0].id) == false)
				continue;

			return panelType;
		}
	}

	return nullptr;
}

void ImGuiPanelUtility::OpenContextPanel(const Varlet::Core::Type* panelType, const char* name, void* context)
{
	auto it = std::find_if(_panels.begin(), _panels.end(), [&panelType, &context](ImGuiPanel* panel)
		{
			if (auto contextPanel = Cast<ImGuiContextPanelBase>(panel))
				if (contextPanel->IsSameContext(context))
					return true;

			return false;
		});

	if (it != _panels.end())
		return;

	const auto newContextPanel = (ImGuiContextPanelBase*)CastMemory(panelType, ImGuiContextPanelBase::GetTypeStatic(), std::bit_cast<int8_t*>(panelType->CreateInstance()));
	assert(newContextPanel);

	newContextPanel->Initialize(name, context);

	_actions.push_back({ newContextPanel, PanelActionType::Add });
}

void ImGuiPanelUtility::OpenInitializationAssetPanel(const Varlet::Core::Type* assetType, std::wstring directory, bool& isSuccessful)
{
	static const uint32_t panelTemplteId = Varlet::Core::Type::GetTemplateId(NAME_OF(InitializationAssetPanel));

	for (auto& panelType : _initializationPanelTypes)
	{
		const Varlet::Core::Type* type = panelType;

		for (auto& info : type->parentInfos)
		{
			if (info.type->module != nullptr)
				continue;

			const auto templateType = dynamic_cast<const Varlet::Core::TemplateType*>(info.type);
			assert(templateType);

			if (templateType->templateId != panelTemplteId)
				continue;

			if (assetType->CheckIsA(templateType->parameterList[0].id) == false)
				continue;

			const auto newPanel = CastMemory<InitializationAssetPanelBase>(panelType, std::bit_cast<int8_t*>(type->CreateInstance()));
			newPanel->Initialize(directory, isSuccessful);

			_actions.push_back({ newPanel, PanelActionType::Add });
			return;
		}
	}

	isSuccessful = false;
}

void ImGuiPanelUtility::UpdatePanels()
{
	ProcessActions();

	for (auto panel : _panels)
	{
		panel->Update();

		if (auto closablePanel = Cast<IClosablePanel>(panel))
		{
			if (closablePanel->ShouldClose())
				_actions.push_back({ panel, PanelActionType::Remove });
		}
	}
}

void ImGuiPanelUtility::InitializeImGui()
{
	const auto mainWindow = Screen::GetNative();

	glfwMakeContextCurrent(mainWindow);
	gladLoadGL();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiPanelUtility::ProcessActions()
{
	if (_actions.empty())
		return;

	for (auto& action : _actions)
	{
		if (action.actionType == PanelActionType::Add)
		{
			if (auto propertyPanel = Cast<ImGuiPropertyPanel>(action.panel))
			{
				_propertyPanels.push_back(propertyPanel);
			}
			else
			{
				const auto imguiPanel = Cast<ImGuiPanel>(action.panel);

				if (action.panel->GetType()->IsA(DockSpace))
					_panels.insert(_panels.begin(), imguiPanel);
				else
					_panels.push_back(imguiPanel);
			}

			action.panel->Initialize();
		}
		else
		{
			_panels.erase(std::remove(_panels.begin(), _panels.end(), action.panel), _panels.end());
			action.panel->Shutdown();
			delete action.panel;
		}
	}

	_actions.clear();
}

bool ImGuiPanelUtility::IsImGuiPanelType(const Varlet::Core::Type* type) const
{
	return type->IsA(ImGuiPanel) || type->IsA(ImGuiPropertyPanel);
}
