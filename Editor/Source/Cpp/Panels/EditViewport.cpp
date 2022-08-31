#include "Panels/EditViewport.h"
#include "EditorCore.h"
#include "VarletFramework.h"

#include "EditorCamera.h"
#include "Rendering/Texture.h"
#include "EditViewport.h"

EditViewport::~EditViewport()
{
	delete _camera;
}

void EditViewport::Init()
{
	_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	_gizmoMode = ImGuizmo::MODE::LOCAL;

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

void EditViewport::UpdateView()
{
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	ImGui::Begin("Viewport", nullptr, windowFlags);

	UpdateControl();

	if (_camera->IsControlled() == false && ImGuizmo::IsOver() == false)
		UpdateSelect();

	const auto currentSize = ImGui::GetContentRegionAvail();

	UpdateContent(currentSize);
	UpdateGizmo(currentSize);
	UpdateCameraResolution(currentSize);

	ImGui::PopStyleVar();
	ImGui::End();
}

void EditViewport::UpdateControl()
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

	if (_camera->IsControlled() == false)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_Q, false))
			_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

		if (ImGui::IsKeyPressed(ImGuiKey_W, false))
			_gizmoOperation = ImGuizmo::OPERATION::ROTATE;

		if (ImGui::IsKeyPressed(ImGuiKey_E, false))
		{
			_gizmoOperation = ImGuizmo::OPERATION::SCALE;
			_gizmoMode = ImGuizmo::MODE::LOCAL;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Z, false) && _gizmoOperation != ImGuizmo::OPERATION::SCALE)
			_gizmoMode = ImGuizmo::MODE::WORLD;

		if (ImGui::IsKeyPressed(ImGuiKey_X, false))
			_gizmoMode = ImGuizmo::MODE::LOCAL;
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

void EditViewport::UpdateContent(const ImVec2& contentSize) const
{
	const auto texture = _camera->GetRendereTexture();
	ImGui::Image(reinterpret_cast<ImTextureID>(texture->GetId()), contentSize, ImVec2(0, 1), ImVec2(1, 0));
}

void EditViewport::UpdateGizmo(const ImVec2& contentSize) const
{
	if (EditorData::selectedEntity == nullptr)
		return;

	const auto transform = EditorData::selectedEntity->GetComponent<Transform>();
	glm::mat4 model = glm::translate(glm::mat4(1.f), transform->position);
	model = model * glm::mat4_cast(transform->rotation);
	model = glm::scale(model, transform->scale);

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, contentSize.x, contentSize.y);

	const auto cameraCore = _camera->GetCore();
	ImGuizmo::Manipulate(glm::value_ptr(cameraCore->GetView()), glm::value_ptr(cameraCore->GetProjection()), _gizmoOperation, _gizmoMode, glm::value_ptr(model));

	if (ImGuizmo::IsUsing())
	{
		glm::vec3 translation, scale, skew;
		glm::quat rotation;
		glm::vec4 perspective;

		if (glm::decompose(model, scale, rotation, translation, skew, perspective))
		{
			transform->position = translation;
			transform->rotation = rotation;
			transform->scale = scale;
		}
	}
}

void EditViewport::UpdateCameraResolution(const ImVec2& resolution) const
{
	static ImVec2 lastSize;

	if (resolution.x != lastSize.x || resolution.y != lastSize.y)
		_camera->OnResize(resolution.x, resolution.y);

	lastSize = resolution;
}
