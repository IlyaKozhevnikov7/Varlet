#pragma once

#include "EditorCoreCore.h"
#include "EditorUtility.h"
#include "Platform/Current.h"
#include "PanelUtility.generated.h"

namespace Varlet::Core
{
	struct ModuleInfo;
};

REFLECTABLE(Abstract)
class EDITORCORE_API PanelUtility : public EditorUtility, public IModuleAddedRemovedCallback
{
	GENERATION

public:

	~PanelUtility() override = default;

	void Initialize() override;

	void Shutdown() override;

protected:

	virtual void OnRegisteredPanelType(const Varlet::Core::Type* type);

	virtual void OnUnregisteredPanelType(const Varlet::Core::Type* type);

private:

	void OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info) override;

	void OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info) override;

	bool IsConcreteEditorPanel(const Varlet::Core::Type* type) const;
};
