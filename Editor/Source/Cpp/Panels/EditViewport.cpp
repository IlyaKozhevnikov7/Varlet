#include "Panels/EditViewport.h"
#include "EditorCore.h"

#include "EditorCamera.h"
#include "Rendering/Texture.h"

void EditViewport::Init()
{
	auto scene = EditorData::context->GetCurrentScene();
	auto camera = scene->CreateEntity();

	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>()->SetActive(true);
	_camera = camera->AddComponent<EditorCamera>();

	_camera->InternalStart();
}

void EditViewport::Update()
{
	_camera->InternalUpdate();

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

	ImGui::SetNextWindowSize(ImVec2(640.f, 480.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	ImGui::Begin("Viewport", nullptr, windowFlags);

	auto texture = _camera->GetRendereTexture();
	texture->Activate(0);

	ImGui::Image(reinterpret_cast<ImTextureID>(texture->GetId()), ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::PopStyleVar();

	static ImVec2 lastSize = ImGui::GetWindowSize();

	if (lastSize.x != ImGui::GetWindowSize().x || lastSize.y != ImGui::GetWindowSize().y)
		_camera->OnResize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

	lastSize = ImGui::GetWindowSize();

	ImGui::End();
}
