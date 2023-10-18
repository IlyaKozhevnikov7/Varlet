#pragma once

#include "Common.h"

namespace Varlet::Core
{
	struct CORE_API CommandLine final
	{
	private:

		static std::vector<std::wstring> _args;

	public:

		static void Initialize(int count, char* values[]);

		static bool HasValue(const wchar_t* value);

		static const std::wstring& Get(int32_t index);
	};
}
