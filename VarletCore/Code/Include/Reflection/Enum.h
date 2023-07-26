#pragma once

#include "Reflection/Type.h"

namespace Varlet::Core
{
	struct EnumValue final
	{
		std::string name;
		size_t value;
	};

	struct CORE_API Enum final
	{
		friend class Type;

	private:

		static std::unordered_map<const Type*, std::vector<EnumValue>> _enumValues;

	public:

		static const std::vector<EnumValue>* GetEnumValues(const Type* type);
	};
}