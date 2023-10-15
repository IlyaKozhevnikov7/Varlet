#pragma once

#include "Common.h"
#include "Serialization/Formatter.h"
#include "BaseUtils/ByteWriter.h"

class CORE_API BinarySerializeContext final : public ISerializeContext
{
private:

	ByteArray& _data;

public:

	BinarySerializeContext(ByteArray& data);

	void AddData(std::vector<int8_t>& data) override;

	void AddData(std::vector<int8_t>&& data) override;

	void AddData(void* data, size_t size) override;
};