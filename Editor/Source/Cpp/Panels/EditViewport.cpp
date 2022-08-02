#include "Panels/EditViewport.h"
#include "EditorCore.h"

#include "EditorCamera.h"
#include "Rendering/Texture.h"

void EditViewport::Init()
{
	auto camera = Scene::CreateEntity();
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>()->SetActive(true);
	_camera = camera->AddComponent<EditorCamera>();

	_camera->InternalStart();
}

void EditViewport::Update()
{
	_camera->InternalUpdate();

	UpdateView();
}

void EditViewport::UpdateView() const
{
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	ImGui::Begin("Viewport", nullptr, windowFlags);

	UpdateControl();

	if (_camera->IsControlled() == false)
		UpdateSelect();

	auto texture = _camera->GetRendereTexture();
	texture->Activate(0);

	const ImVec2 currentSize = ImGui::GetContentRegionAvail();

	ImGui::Image(reinterpret_cast<ImTextureID>(texture->GetId()), currentSize, ImVec2(0, 1), ImVec2(1, 0));

	static ImVec2 lastSize;

	if (currentSize.x != lastSize.x || currentSize.y != lastSize.y)
		_camera->OnResize(currentSize.x, currentSize.y);

	lastSize = currentSize;

	ImGui::PopStyleVar();
	ImGui::End();
}

void EditViewport::UpdateControl() const
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		VarletAPI::SetCursorState(CursorState::Disabled);
		_camera->SetControl(true);
	}

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		VarletAPI::SetCursorState(CursorState::Visible);
		_camera->SetControl(false);
	}
}

void EditViewport::UpdateSelect() const
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
	{
		const ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
		const ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 mousePos = ImGui::GetMousePos();

		const ImVec2 pixelPos =
		{
			mousePos.x - windowPos.x - regionMin.x,
			ImGui::GetWindowSize().y - mousePos.y + windowPos.y
		};

		int8_t* pixelInfo = _camera->ReadSelectedPixel(pixelPos.x, pixelPos.y);
		int32_t id = pixelInfo[0] + pixelInfo[1] * 256 + pixelInfo[2] * 256 * 256;

		auto find = Scene::Find([&id](const Entity* entity)
			{
				if (auto meshRenderer = entity->GetComponent<MeshRenderer>())
					return meshRenderer->GetRenderId() == id;

				return false;
			});

		EditorData::selectedEntity = find;
	}
}
