#pragma once

#include "Internal/EditorUtilitySystemInternal.h"

class EditorUtilitySystem final
{
public:

	template<typename T>
	static T* FindByType()
	{
		return Cast<T>(EditorUtilitySystemInternal::FindByType(T::GetTypeStatic()));
	}
};