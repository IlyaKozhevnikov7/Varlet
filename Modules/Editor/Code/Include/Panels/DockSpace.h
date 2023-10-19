#pragma once

#include "Panels/ImGuiPanel.h"
#include "DockSpace.generated.h"

REFLECTABLE()
class DockSpace final : public ImGuiPanel
{
	GENERATION

private:

	std::wstring _scenePath;

public:

	void Initialize() override;

	void Update() override;
};

