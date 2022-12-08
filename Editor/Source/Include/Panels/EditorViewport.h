#pragma once

#include "Panel.h"

namespace ImGuizmo
{
	enum OPERATION;
	enum MODE;
}

class EditorCamera;
struct ImVec2;

class EditorViewport final : public Panel
{
private:

	EditorCamera* _camera;
	ImGuizmo::OPERATION _gizmoOperation;
	ImGuizmo::MODE _gizmoMode;

public:

	~EditorViewport() override;

	void Init() override;

	void Update() override;

private:

	void UpdateView();

	void UpdateControl();

	void UpdateSelect() const;

	void UpdateContent(const ImVec2& contentSize) const;

	void UpdateGizmo(const ImVec2& contentSize) const;

	void UpdateCameraResolution(const ImVec2& resolution) const;
};

