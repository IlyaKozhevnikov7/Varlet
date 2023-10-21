#include "Development/Editor/Panels/SceneViewport.h"
#include "EditorData.h"
#include "Entity.h"
#include "RenderPipelineStages/RenderPipelineStage.h"
#include "RenderPipelineStages/BloomRenderPipelineStage.h"
#include "RenderPipelineSystem.h"
#include "../../../GraphicsInterface/Code/Include/GraphicsAPI.h"
#include "Components/Transform.h"
#include "UIService.h"

void ViewportCamera::Start(SceneViewport* viewport)
{
	_viewport = viewport;

	_pipeline = RenderPipelineStage::Create(RenderPipelineStage::DefaultRenderPipeline);
	_pipeline->next = RenderPipelineStage::Create(BloomRenderPipelineStage::GetTypeStatic());

	RenderPipelineSystem::CreateRenderContext({ 1920, 1080, { &_view, &_projection, &_position } }, _pipeline);

	_isActive = true;
	_isControlled = false;
	_position = glm::vec3(0.f);
	_rotation = glm::vec3(1.f, 0.f, 0.f);

	sensitivity = 0.15f;
	speed = 20.f;
	
	OnResize(1920, 1080);

	CulculateView();
}

void ViewportCamera::Update()
{
	if (_isControlled)
		UpdateMoveAndRotation();
}

void ViewportCamera::UpdateMoveAndRotation()
{
	const ImVec2 mousePos = ImGuiAPI::GetMousePos();
	const glm::vec2 delta = glm::vec2(_lastMousePos.y - mousePos.y, _lastMousePos.x - mousePos.x) * sensitivity;
	const float deltaTime = Time::GetDeltaTime();
	
	_rotation += glm::vec3(delta, 0.f);
	
	_lastMousePos = mousePos; 
	
	const glm::quat orientation = glm::quat(glm::radians(_rotation));
	
	if (ImGuiAPI::IsKeyDown(ImGuiKey_A))
		_position += -(orientation * glm::vec3(1.f, 0.f, 0.f)) * speed * deltaTime;
	
	if (ImGuiAPI::IsKeyDown(ImGuiKey_D))
		_position += orientation * glm::vec3(1.f, 0.f, 0.f) * speed * deltaTime;
	
	if (ImGuiAPI::IsKeyDown(ImGuiKey_W))
		_position += -(orientation * glm::vec3(0.f, 0.f, 1.f)) * speed * deltaTime;
	
	if (ImGuiAPI::IsKeyDown(ImGuiKey_S))
		_position += orientation * glm::vec3(0.f, 0.f, 1.f) * speed * deltaTime;

	CulculateView();
}

void ViewportCamera::SetControl(const bool& control)
{
	_isControlled = control;
	
	if (_isControlled)
		_lastMousePos = ImGuiAPI::GetMousePos();
}

bool ViewportCamera::IsControlled() const
{
	return _isControlled;
}

const void* ViewportCamera::GetRendereTexture()
{
	return Varlet::Graphics::GetNativeTexture(&_pipeline->mappedAttachments[0]);
}

void ViewportCamera::OnResize(const int32_t& width, const int32_t& height)
{
	_resolution = { width, height };
	_projection = glm::perspective(glm::radians(45.f), _resolution.x / _resolution.y, 0.1f, 10000.f);
}

const glm::mat4& ViewportCamera::GetView() const
{
	return _view;
}

const glm::mat4& ViewportCamera::GetProjection() const
{
	return _projection;
}

void ViewportCamera::CulculateView()
{
	const glm::mat4 translateMat = glm::translate(glm::mat4(1.f), -_position);
	const glm::mat4 orientationMat = glm::mat4_cast(glm::inverse(glm::quat(glm::radians(_rotation))));

	_view = orientationMat * translateMat;
}

void SceneViewport::Initialize()
{
	_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	_gizmoMode = ImGuizmo::MODE::LOCAL;

	_camera.Start(this);
}

void SceneViewport::Update()
{
	_camera.Update();

	DrawViewport();
}

void SceneViewport::DrawViewport()
{
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
	ImGuiAPI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	// Draw viewport

	ImGuiAPI::Begin("Viewport", nullptr, windowFlags);

	UpdateControl();

	if (_camera.IsControlled() == false && ImGuiAPI::GizmosIsOver() == false)
		UpdateSelect();

	const auto currentSize = ImGuiAPI::GetContentRegionAvail();

	UpdateContent(currentSize);
	UpdateGizmo(currentSize);
	UpdateCameraResolution(currentSize);

	ImGuiAPI::PopStyleVar();
	ImGuiAPI::End();

	// Draw camera settings

	ImGuiAPI::Begin("Editor Camera Settings", nullptr, windowFlags);

	ImGuiAPI::DragFloat("Sensitivity", &_camera.sensitivity);
	ImGuiAPI::DragFloat("Speed", &_camera.speed);

	static BloomRenderPipelineStage* bloomRS = Cast<BloomRenderPipelineStage>(_camera._pipeline->next);

	ImGuiAPI::DragFloat("Filter Radius", &bloomRS->_filterRadius);
	ImGuiAPI::DragFloat("Another Filter Radius", &bloomRS->_anotherFilterRadius);
	ImGuiAPI::DragFloat("Exposure", &bloomRS->exposure);
	ImGuiAPI::DragFloat("Bloom Strength", &bloomRS->bloomStrength);

	UIService::DisplayValue("Render Pipeline", _camera._pipeline->GetType()->id, (int8_t*)&_camera._pipeline, Varlet::Core::Pointer);

	ImGuiAPI::End();
}

void SceneViewport::UpdateControl()
{
	if (ImGuiAPI::IsMouseClicked(ImGuiMouseButton_Right) && ImGuiAPI::IsWindowHovered())
		_camera.SetControl(true);

	if (ImGuiAPI::IsMouseReleased(ImGuiMouseButton_Right))
		_camera.SetControl(false);

	if (_camera.IsControlled() == false)
	{
		if (ImGuiAPI::IsKeyPressed(ImGuiKey_Q, false))
			_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

		if (ImGuiAPI::IsKeyPressed(ImGuiKey_W, false))
			_gizmoOperation = ImGuizmo::OPERATION::ROTATE;

		if (ImGuiAPI::IsKeyPressed(ImGuiKey_E, false))
		{
			_gizmoOperation = ImGuizmo::OPERATION::SCALE;
			_gizmoMode = ImGuizmo::MODE::LOCAL;
		}

		if (ImGuiAPI::IsKeyPressed(ImGuiKey_Z, false) && _gizmoOperation != ImGuizmo::OPERATION::SCALE)
			_gizmoMode = ImGuizmo::MODE::WORLD;

		if (ImGuiAPI::IsKeyPressed(ImGuiKey_X, false))
			_gizmoMode = ImGuizmo::MODE::LOCAL;
	}
	else
	{
		ImGuiAPI::SetMouseCursor(ImGuiMouseCursor_None);
	}
}

void SceneViewport::UpdateSelect() const
{
	if (ImGuiAPI::IsMouseClicked(ImGuiMouseButton_Left) && ImGuiAPI::IsWindowHovered())
	{
	}
}

void SceneViewport::UpdateContent(const ImVec2& contentSize)
{
	ImGuiAPI::Image(NATIVE_TEXTURE_TO_IMTEXTUREID(_camera.GetRendereTexture()), contentSize, ImVec2(0, 1), ImVec2(1, 0));
}

void SceneViewport::UpdateGizmo(const ImVec2& contentSize) const
{
	if (EditorData::SelectedEntity == nullptr)
		return;
	
	const auto transform = EditorData::SelectedEntity->GetComponent<Transform>();
	if (transform == nullptr)
		return;
	
	glm::mat4 model = transform->GetModelMatrix();
	const ImVec2 windowPos = ImGuiAPI::GetWindowPos();

	ImGuiAPI::GizmosSetOrthographic(false);
	ImGuiAPI::GizmosSetDrawlist();
	ImGuiAPI::GizmosSetRect(windowPos.x, windowPos.y, contentSize.x, contentSize.y);

	ImGuiAPI::GizmosManipulate(glm::value_ptr(_camera.GetView()), glm::value_ptr(_camera.GetProjection()), _gizmoOperation, _gizmoMode, glm::value_ptr(model));

	if (ImGuiAPI::GizmosIsUsing())
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

void SceneViewport::UpdateCameraResolution(const ImVec2& resolution)
{
	static ImVec2 lastSize;
	
	if (resolution.x != lastSize.x || resolution.y != lastSize.y)
		_camera.OnResize(resolution.x, resolution.y);
	
	lastSize = resolution;
}
