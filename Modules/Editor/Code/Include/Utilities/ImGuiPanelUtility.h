#pragma once

#include "EditorCore.h"
#include "PanelUtility.h"
#include "ImGuiPanelUtility.generated.h"

enum class PanelActionType : uint8_t
{
	Add = 0,
	Remove
};

struct PanelAction final
{
	EditorPanel* panel;
	PanelActionType actionType;
};

REFLECTABLE()
class EDITOR_API ImGuiPanelUtility final : public PanelUtility, public IUpdatableEditorUtility
{
	GENERATION

private:

	std::vector<PanelAction> _actions;
	std::vector<ImGuiPanel*> _panels;

	std::vector<ImGuiPropertyPanel*> _propertyPanels;
	std::shared_ptr<DefaultImGuiPropertyPanel> _defaultPropertyPanel;

	std::vector<TypeOf<ImGuiContextPanelBase>> _contextPanelTypes;
	std::vector<TypeOf<InitializationAssetPanelBase>> _initializationPanelTypes;

public:

	~ImGuiPanelUtility() override = default;

	void Initialize() override;

	void Update() override;
	
	void Shutdown() override;

	void DisplayProperty(const Varlet::Core::Type* type, const char* name, void* context, bool isPointer);

	const Varlet::Core::Type* GetContextPanelFor(const Varlet::Core::Type* contextType);

	void OpenContextPanel(const Varlet::Core::Type* panelType, const char* name, void* context);

	void OpenInitializationAssetPanel(const Varlet::Core::Type* assetType, std::wstring directory, bool& isSuccessful);

private:

	virtual void OnRegisteredPanelType(const Varlet::Core::Type* type);

	virtual void OnUnregisteredPanelType(const Varlet::Core::Type* type);

	void UpdatePanels();

	void InitializeImGui();

	void ProcessActions();

	bool IsImGuiPanelType(const Varlet::Core::Type* type) const;
};
