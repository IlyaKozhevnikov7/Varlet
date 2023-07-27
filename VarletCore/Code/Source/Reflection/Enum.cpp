#include "Reflection/Enum.h"

namespace Varlet::Core
{
	std::unordered_map<const Type*, std::vector<EnumValue>> Enum::_enumValues;

	const std::vector<EnumValue>* Enum::GetEnumValues(const Type* type)
	{
		return _enumValues.contains(type)
			? &_enumValues[type]
			: nullptr;
	}
}
