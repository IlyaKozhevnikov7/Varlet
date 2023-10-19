#pragma once

#include "Panels/ImGuiPanel.h"
#include "Assets/Shader.h"
#include "ShaderPanel.generated.h"

REFLECTABLE()
class ShaderContextPanel final : public ImGuiContextPanel<Shader>
{
	GENERATION

public:

	~ShaderContextPanel() override = default;

	void Update() override;
};

REFLECTABLE()
class ShaderInitializationPanel final : public InitializationAssetPanel<Shader>
{
	GENERATION

public:

	~ShaderInitializationPanel() override = default;

	void Update() override;

private:

	void CreateNativeShaderFile(const std::wstring& name);
};
