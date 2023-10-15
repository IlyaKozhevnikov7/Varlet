#pragma once

#include "Common.h"
#include "Reflection/Type.h"

struct SharedDependencies final
{
	const Varlet::Core::Type* referenceType = nullptr;
	void** reference;
};

struct SharedResourceInfo final
{
	std::vector<int8_t> id;
	std::vector<SharedDependencies> dependencies;
};

class CORE_API ISerializeContext
{
public:

	virtual void AddData(std::vector<int8_t>& data) = 0;

	virtual void AddData(std::vector<int8_t>&& data) = 0;

	virtual void AddData(void* data, size_t size) = 0;

	template<typename T, size_t TSize = sizeof(T)>
	void AddData(T data)
	{
		AddData(&data, TSize);
	};
};

class CORE_API IDeserializeContext
{
public:

	virtual std::vector<int8_t> ReadAll() = 0;

	virtual uint32_t GetDataSize() = 0;

	virtual void* Read(size_t size) = 0;

	template<typename T, size_t TSize = sizeof(T)>
	T Read()
	{
		return *std::bit_cast<T*>(Read(TSize));
	}
};
