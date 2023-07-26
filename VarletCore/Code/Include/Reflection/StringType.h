#pragma once

#include "Reflection/Type.h"

namespace Varlet::Core
{
	struct CORE_API StringType final : public Type
	{
	public:

		virtual ~StringType() = default;

		StringType();

		size_t GetSize(void* instance) const;

		const int8_t* GetData(void* instance) const;
	};
}