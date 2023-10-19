#pragma once

#include "EditorCore.h"

struct EDITOR_API EditorData final
{
public:

	static std::wstring ProjectName;
	static std::wstring ProjectPath;
	static Entity* SelectedEntity;
	static std::vector<const Varlet::Core::Type*> ContextComponentTypes;
	static std::atomic_bool IsReloading;

	static Varlet::Core::Module* Module;
};
