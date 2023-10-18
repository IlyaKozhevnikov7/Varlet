#include "EditorUtility.h"
#include "ModuleService.h"

void EditorUtility::Initialize()
{
}

void EditorUtility::Shutdown()
{
}

void IModuleAddedRemovedCallback::Initialize()
{
	Varlet::Core::ModuleService::OnRegisteredNewModule.Bind(this, &ThisClass::OnRegisteredNewModule);
	Varlet::Core::ModuleService::OnUnregisteredNewModule.Bind(this, &ThisClass::OnUnregisteredNewModule);
}

void IModuleAddedRemovedCallback::Shutdown()
{
	Varlet::Core::ModuleService::OnRegisteredNewModule.UnBind(this, &ThisClass::OnRegisteredNewModule);
	Varlet::Core::ModuleService::OnUnregisteredNewModule.UnBind(this, &ThisClass::OnUnregisteredNewModule);
}
