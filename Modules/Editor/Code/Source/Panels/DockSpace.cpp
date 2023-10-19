#include "Panels/DockSpace.h"
#include "EditorUtils.h"
#include "Scene.h"
#include "SceneUtils.h"
#include "EditorData.h"

void DockSpace::Initialize()
{
	_scenePath = EditorData::ProjectPath + WIDE("\\Assets\\DefaultScene.vscene");
}

void DockSpace::Update()
{
	constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar
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
	
	ImGui::Begin("DockSpace", nullptr, windowFlags);
	ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2(0.0f, 0.0f), dockspaceFlags);
	
	ImGui::PopStyleVar(3);
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Entities"))
		{
			if (ImGui::MenuItem("Add entity"))
				EditorUtils::CreateDefautlEntity();

			if (ImGui::MenuItem("Add empty entity"))
				Scene::CreateEntity();
	
			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Save"))
			{
				Varlet::SceneUtils::Save(_scenePath.c_str());
			}
	
			if (ImGui::MenuItem("Load"))
			{
				EditorData::SelectedEntity = nullptr;
				Varlet::SceneUtils::Load(_scenePath.c_str());
			}
		
			ImGui::EndMenu();
		}
	
		ImGui::EndMenuBar();
	}
	
	ImGui::End();
}
