#include "CommandLine.h"
#include "Platform/Current.h"

namespace Varlet::Core
{
	std::vector<std::wstring> CommandLine::_args;

	void CommandLine::Initialize(int count, char* values[])
	{
		assert(_args.size() == 0);

		_args.resize(count);

		for (int i = 0; i < count; i++)
		{
			std::cout << values[i] << std::endl;
			_args[i] = Varlet::Core::Platform::ConvertToWide(values[i]);
		}
	}

	bool CommandLine::HasValue(const wchar_t* value)
	{
		assert(value != nullptr);

		for (auto& arg : _args)
			if (arg == value)
				return true;

		return false;
	}

	const std::wstring& CommandLine::Get(int32_t index)
	{
		assert(index >= 0);

		if (index >= _args.size())
			return WIDE("");

		return _args[index];
	}
}
