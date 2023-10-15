#pragma once

#include "BaseUtils/ByteSequence.h"

class CORE_API ByteWriter final : public Varlet::Core::ByteSequence
{
public:

	ByteWriter(void* data, size_t size);

	template<typename T, size_t TSize = sizeof(T)>
	void Write(T& value)
	{
		Write((void*)&value, TSize);
	}

	template<typename T, size_t TSize = sizeof(T)>
	void Write(T&& value)
	{
		Write((void*)&value, TSize);
	}

	void Write(void* value, size_t size);
};

class CORE_API ByteArray final
{
private:

	std::vector<int8_t> _data;

public:

	ByteArray(size_t capacity);

	size_t Size();

	std::vector<int8_t> ExtractData();

	template<typename T, size_t TSize = sizeof(T)>
	void Write(T& value)
	{
		Write((void*)&value, TSize);
	}

	template<typename T, size_t TSize = sizeof(T)>
	void Write(T&& value)
	{
		Write((void*)&value, TSize);
	}

	template<typename T, size_t TSize = sizeof(T)>
	void Write(T& value, size_t position)
	{
		Write((void*)&value, TSize, position);
	}

	template<typename T, size_t TSize = sizeof(T)>
	void Write(T&& value, size_t position)
	{
		Write((void*)&value, TSize, position);
	}

	void Write(void* value, size_t size);

	void Write(void* value, size_t size, size_t position);

	void SkipRange(size_t size);
};
