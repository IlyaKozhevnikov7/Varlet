#pragma once

#include "EditorCoreCore.h"
#include "EditorUtility.h"

class EditorUtilitySystemInternal final
{
private:

	static std::vector<EditorUtility*>* _utilities;

public:

	static void Initialize(std::vector<EditorUtility*>& utilities);

	EDITORCORE_API static EditorUtility* FindByType(const Varlet::Core::Type* type);
};
