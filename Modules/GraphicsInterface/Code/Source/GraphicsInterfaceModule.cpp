#include "GraphicsInterfaceModule.h"
#include "ModuleService.h"

bool GraphicsInterfaceModule::Initialize()
{
	const auto nativeModuleName = WIDE("OpenGL");
	Varlet::Core::ModuleService::LoadModule(nativeModuleName);

	return true;
}
