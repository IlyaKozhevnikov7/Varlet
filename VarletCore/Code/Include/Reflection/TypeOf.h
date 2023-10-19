#pragma once

#include "Reflection/Type.h"
#include "Serialization/SerializationUtils.h"
#include "Serialization/Formatter.h"
#include "TypeOf.generated.h"

REFLECTABLE()
template<typename T>
struct TypeOf final : public ICustomSerializable
{
	GENERATION

private:

	FIELD()
	const Varlet::Core::Type* _type;

public:

	TypeOf() :
		_type(nullptr)
	{
	}

	TypeOf(const Varlet::Core::Type* type)
	{
		if (type != nullptr && type->IsA(T))
		{
			_type = type;
			return;
		}

		_type = type;
	}

	operator const Varlet::Core::Type*() const
	{
		return _type;
	}

	operator bool() const
	{
		return _type != nullptr;
	}

	bool operator==(const ThisClass& other) const
	{
		return _type == other._type;
	}

	bool operator==(const Varlet::Core::Type* type) const
	{
		return _type == type;
	}

	void Serialize(ISerializeContext* context) override
	{
		const uint32_t value = _type == nullptr
			? 0
			: _type->id;
		
		context->AddData<uint32_t>(value);
	}

	bool Deserialize(IDeserializeContext* context) override
	{
		uint32_t id = context->Read<uint32_t>();
		_type = Varlet::Core::Type::GetType(id);

		return true;
	}
};
