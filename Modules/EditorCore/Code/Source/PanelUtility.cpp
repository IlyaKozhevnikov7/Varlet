#include "PanelUtility.h"
#include "ModuleService.h"
#include "EditorPanel.h"

void PanelUtility::Initialize()
{
	IModuleAddedRemovedCallback::Initialize();

	auto types = Varlet::Core::Type::GetAllTypes();
	for (auto& type : types)
		if (IsConcreteEditorPanel(type))
			OnRegisteredPanelType(type);
}

void PanelUtility::Shutdown()
{
	IModuleAddedRemovedCallback::Shutdown();
}

void PanelUtility::OnRegisteredPanelType(const Varlet::Core::Type* type)
{
}

void PanelUtility::OnUnregisteredPanelType(const Varlet::Core::Type* type)
{
}

void PanelUtility::OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info)
{
	auto moduleTypes = Varlet::Core::Type::GetTypes(info.module);

	for (auto type : moduleTypes)
		if (IsConcreteEditorPanel(type))
			OnRegisteredPanelType(type);
}

void PanelUtility::OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info)
{
	auto moduleTypes = Varlet::Core::Type::GetTypes(info.module);

	for (auto type : moduleTypes)
		if (IsConcreteEditorPanel(type))
			OnUnregisteredPanelType(type);
}

bool PanelUtility::IsConcreteEditorPanel(const Varlet::Core::Type* type) const
{
	return type->flags.NoHas(Varlet::Core::TypeMetaFlags::Abstract) && type->IsA(EditorPanel);
}