#pragma once

#include "Panels/ImGuiPanel.h"
#include "SceneTree.generated.h"

class Scene;

REFLECTABLE()
class SceneTree final : public ImGuiPanel
{
	GENERATION

private:

	const Scene* _currnetScene;

public:

	void Initialize() override;

	void Update() override;

private:

	void OnSceneChanged(const Scene* newScene);
};

