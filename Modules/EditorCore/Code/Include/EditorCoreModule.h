#pragma once

#include "EditorCoreCore.h"
#include "EditorUtility.h"
#include "Module.h"

namespace Varlet::Core
{
	struct ModuleInfo;
};

class EditorCoreModule final : public Varlet::Core::Module, public Varlet::Core::IUpdatebleModule, public IModuleAddedRemovedCallback
{
private:

	enum class UtilityActionType : uint8_t
	{
		Add = 0,
		Remove
	};

	struct UtilityAction final
	{
		EditorUtility* utility;
		UtilityActionType actionType;
	};

	std::vector<UtilityAction> _actions;
	std::vector<EditorUtility*> _utilities;

public:

	~EditorCoreModule() override = default;

	bool Initialize() override;

	void Shutdown() override;

	void Update() override;

private:

	void ProcessActions();

	void OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info) override;

	void OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info) override;
};
