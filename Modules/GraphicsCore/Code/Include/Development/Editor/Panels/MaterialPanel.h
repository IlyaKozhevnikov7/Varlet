#pragma once

#include "Panels/ImGuiPanel.h"
#include "Assets/Material.h"
#include "MaterialPanel.generated.h"

REFLECTABLE()
class MaterialContextPanel final : public ImGuiContextPanel<Material>
{
	GENERATION

public:

	~MaterialContextPanel() override = default;

	void Update() override;
};

REFLECTABLE()
class MaterialInitializationPanel final : public InitializationAssetPanel<Material>
{
	GENERATION

private:

	constexpr static int32_t MaxNameLength = 128;

	char _materialName[MaxNameLength];
	int32_t _selectedShaderIndex = 0;
	std::vector<std::wstring_view> _shaderPaths;
	std::vector<std::string> _shaderNames;

public:

	~MaterialInitializationPanel() override = default;

	void Initialize() override;

	void Update() override;

private:

	void DrawCreateMaterialMenu();

	void DrawNoShaderMenu();

};
