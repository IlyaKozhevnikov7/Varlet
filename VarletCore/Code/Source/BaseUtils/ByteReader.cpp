#include "BaseUtils/ByteReader.h"

ByteReader::ByteReader(void* data, size_t size) :
	Varlet::Core::ByteSequence(data, size)
{
}

bool ByteReader::HasData() const
{
	return GetPosition() < GetSize();
}

void* ByteReader::Read(size_t size)
{
	void* data = GetData();
	MovePosition(size);

	return data;
}
