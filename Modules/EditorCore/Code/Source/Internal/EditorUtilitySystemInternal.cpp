#include "Internal/EditorUtilitySystemInternal.h"

std::vector<EditorUtility*>* EditorUtilitySystemInternal::_utilities;

void EditorUtilitySystemInternal::Initialize(std::vector<EditorUtility*>& utilities)
{
	_utilities = &utilities;
}

EditorUtility* EditorUtilitySystemInternal::FindByType(const Varlet::Core::Type* type)
{
	for (auto& utility : *_utilities)
	{
		if (utility->GetType()->CheckIsA(type))
			return utility;
	}

	return nullptr;
}
