#pragma once

#include "EditorCoreCore.h"
#include "EditorUtility.generated.h"

REFLECTABLE(Abstract)
class EDITORCORE_API EditorUtility
{
	GENERATION

public:

	virtual ~EditorUtility() = default;

	virtual void Initialize();

	virtual void Shutdown();
};

REFLECTABLE(Interface)
class EDITORCORE_API IUpdatableEditorUtility
{
	GENERATION

public:

	virtual ~IUpdatableEditorUtility() = default;
	
	virtual void Update() = 0;
};

namespace Varlet::Core
{
	struct ModuleInfo;
};

REFLECTABLE(Interface)
class EDITORCORE_API IModuleAddedRemovedCallback
{
	GENERATION

public:

	virtual ~IModuleAddedRemovedCallback() = default;

	void Initialize();

	void Shutdown();

	virtual void OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info) = 0;

	virtual void OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info) = 0;
};
