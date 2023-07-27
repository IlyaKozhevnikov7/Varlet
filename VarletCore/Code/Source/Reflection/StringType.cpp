#include "Reflection/StringType.h"

namespace Varlet::Core
{
	StringType::StringType() :
		Type("std::string", sizeof(std::string), {}, {}, nullptr, nullptr)
	{
	}

	size_t StringType::GetSize(void* instance) const
	{
		return std::bit_cast<std::string*>(instance)->size();
	}

	const int8_t* StringType::GetData(void* instance) const
	{
		return std::bit_cast<int8_t*>(std::bit_cast<std::string*>(instance)->c_str());
	}
}