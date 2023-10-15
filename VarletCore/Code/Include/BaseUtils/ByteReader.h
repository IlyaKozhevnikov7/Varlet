#pragma once

#include "BaseUtils/ByteSequence.h"

class CORE_API ByteReader final : public Varlet::Core::ByteSequence
{
public:

	ByteReader(void* data, size_t size);

	bool HasData() const;

	template<typename T, size_t TSize = sizeof(T)>
	T Read()
	{
		return *std::_Bit_cast<T*>(Read(TSize));
	}

	void* Read(size_t size = 0);
};
