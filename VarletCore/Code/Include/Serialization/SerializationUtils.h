#pragma once

#include "Common.h"
#include "SerializationUtils.generated.h"

namespace Varlet::Core
{
	struct SerializeInfo;
	struct DeserializeInfo;
}

REFLECTABLE(Interface)
class CORE_API ICustomSerializable
{
	GENERATION

public:

	virtual ~ICustomSerializable() = default;

	virtual void Serialize(Varlet::Core::SerializeInfo& info) = 0;

	virtual void Deserialize(Varlet::Core::DeserializeInfo& info) = 0;

	bool CheckIsA(const Varlet::Core::Type* type) const
	{
		return GetType()->CheckIsA(type);
	}
};

REFLECTABLE(Interface)
class ISharedSerializable
{
	GENERATION

public:

	virtual ~ISharedSerializable() = default;

	virtual std::vector<int8_t> GetSharedId() = 0;
};
