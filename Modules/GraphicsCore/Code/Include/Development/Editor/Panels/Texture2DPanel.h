#pragma once

#include "Panels/ImGuiPanel.h"
#include "Assets/Texture2D.h"
#include "Texture2DPanel.generated.h"

REFLECTABLE()
class Texture2DPropertyPanel final : public ImGuiPropertyPanel
{
	GENERATION

public:

	void Display(const char* name, void* context, bool isPointer) override;

	const Varlet::Core::Type* GetContextType() override;
};

REFLECTABLE()
class Texture2DInitializationPanel final : public InitializationAssetPanel<Texture2D>
{
	GENERATION

public:

	~Texture2DInitializationPanel() override = default;

	void Update() override;
};
