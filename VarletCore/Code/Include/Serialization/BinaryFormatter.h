#pragma once

#include "Common.h"
#include "Serialization/Formatter.h"

class CORE_API BinaryFormatter final
{
public:

	template<typename T>
	static std::vector<int8_t> Serialize(T* value)
	{
		assert(value);

		return SerializeInternal(value, value->GetType());
	}

	template<typename T>
	static T* Deserialize(std::vector<int8_t>& data, std::vector<SharedResourceInfo>* sharedResources = nullptr)
	{
		return (T*)DeserializeInternal(data.data(), data.size(), sharedResources);
	}

private:

	static std::vector<int8_t> SerializeInternal(void* value, const Varlet::Core::Type* type);

	static void* DeserializeInternal(void* data, size_t size, std::vector<SharedResourceInfo>* sharedResources);
};
