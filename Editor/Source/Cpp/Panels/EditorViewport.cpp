#include "EditorViewport.h"
#include "EditorCore.h"
#include "VarletFramework.h"
#include "EditorCamera.h"
#include "Rendering/Texture.h"

EditorViewport::~EditorViewport()
{
	delete _camera;
}

void EditorViewport::Init()
{
	_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	_gizmoMode = ImGuizmo::MODE::LOCAL;
	
	EditorData::editorCamera = Scene::CreateEntity();
	EditorData::editorCamera->AddComponent<Transform>();
	EditorData::editorCamera->AddComponent<Camera>();
	_camera = EditorData::editorCamera->AddComponent<EditorCamera>();
	
	_camera->InternalStart();
}

void EditorViewport::Update()
{
	_camera->InternalUpdate();

	UpdateView();
}

void EditorViewport::UpdateView()
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

void EditorViewport::UpdateControl()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		_camera->SetControl(true);
	}

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
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

void EditorViewport::UpdateSelect() const
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

		std::vector<uint8_t> pixelInfo = _camera->ReadSelectedPixel(pixelPos.x, pixelPos.y);
		uint32_t id = pixelInfo[0]
			| pixelInfo[1] << 8
			| pixelInfo[2] << 16
			| pixelInfo[3] << 24;
		
		auto find = Scene::Find([&id](Entity* entity)
			{
				return entity->GetId() == id;
			});
		
		EditorData::selectedEntity = find;
	}
}

void EditorViewport::UpdateContent(const ImVec2& contentSize) const
{
	ImGui::Image(NATIVE_TEXTURE_TO_IMTEXTUREID(_camera->GetRendereTexture()), contentSize, ImVec2(0, 1), ImVec2(1, 0));
}

void EditorViewport::UpdateGizmo(const ImVec2& contentSize) const
{
	if (EditorData::selectedEntity == nullptr)
		return;

	const auto transform = EditorData::selectedEntity->GetComponent<Transform>();
	glm::mat4 model = transform->GetModelMatrix();

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
			transform->rotation = glm::degrees(glm::eulerAngles(rotation));
			transform->scale = scale;
		}
	}
}

void EditorViewport::UpdateCameraResolution(const ImVec2& resolution) const
{
	static ImVec2 lastSize;

	if (resolution.x != lastSize.x || resolution.y != lastSize.y)
		_camera->OnResize(resolution.x, resolution.y);

	lastSize = resolution;
}
