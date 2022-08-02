#pragma once

#include "Panel.h"

namespace Varlet
{
	class Scene;
}

class SceneTree final : public Panel
{
private:

	const Varlet::Scene* _currnetScene;

public:

	void Init() override;

	void Update() override;

private:

	void OnSceneChanged(const Varlet::Scene* newScene);
};

