#pragma once

#include "Common.h"
#include "SerializationUtils.generated.h"

class ISerializeContext;
class IDeserializeContext;

REFLECTABLE(Interface)
class CORE_API ICustomSerializable
{
	GENERATION

public:

	virtual ~ICustomSerializable() = default;

	virtual void Serialize(ISerializeContext* context) = 0;

	/*
	*	The result determines whether the object was deserialized correctly or not. 
	*	If false is returned, the object will be deleted.
	*/
	virtual bool Deserialize(IDeserializeContext* context) = 0;
};

REFLECTABLE(Interface)
class ISharedSerializable
{
	GENERATION

public:

	virtual ~ISharedSerializable() = default;

	virtual std::vector<int8_t> GetSharedId() = 0;
};
