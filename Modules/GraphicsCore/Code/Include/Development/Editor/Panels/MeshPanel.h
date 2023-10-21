#pragma once

#include "Panels/ImGuiPanel.h"
#include "Assets/Mesh.h"
#include "MeshPanel.generated.h"

REFLECTABLE()
class MeshInitializationPanel final : public InitializationAssetPanel<Mesh>
{
	GENERATION

public:

	~MeshInitializationPanel() override = default;

	void Update() override;
};
