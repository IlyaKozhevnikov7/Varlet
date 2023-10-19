#pragma once

#include "GraphicsCoreCore.h"
#include "Panels/ImGuiPanel.h"
#include "SceneViewport.generated.h"

class RenderPipelineStage;
class SceneViewport;

class ViewportCamera final
{
public:

	float sensitivity;
	float speed;

	RenderPipelineStage* _pipeline;

private:

	bool _isControlled;
	bool _isActive;

	glm::mat4 _view;
	glm::mat4 _projection;
	glm::vec2 _resolution;

	glm::vec3 _position;
	glm::vec3 _rotation;

	void* _nativeTexture;

	ImVec2 _lastMousePos;

	SceneViewport* _viewport;

public:

	void Start(SceneViewport* viewport);

	void Update();

	void SetControl(const bool& control);

	bool IsControlled() const;

	const void* GetRendereTexture();

	void OnResize(const int32_t& width, const int32_t& height);

	const glm::mat4& GetView() const;

	const glm::mat4& GetProjection() const;

private:

	void UpdateMoveAndRotation();

	void CulculateView();
};

REFLECTABLE()
class SceneViewport final : public ImGuiPanel
{
	GENERATION

private:

	ViewportCamera _camera;
	ImGuizmo::OPERATION _gizmoOperation;
	ImGuizmo::MODE _gizmoMode;

public:

	~SceneViewport() override = default;

	void Initialize() override;

	void Update() override;

private:

	void DrawViewport();

	void UpdateControl();

	void UpdateSelect() const;

	void UpdateContent(const ImVec2& contentSize);

	void UpdateGizmo(const ImVec2& contentSize) const;

	void UpdateCameraResolution(const ImVec2& resolution);
};
