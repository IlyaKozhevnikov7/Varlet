#pragma once

#include "Reflection/Type.h"
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

	void Serialize(Varlet::Core::SerializeInfo& info) override
	{
		if (_type == nullptr)
			info.AddData<uint32_t>(0);
		else
			info.AddData(_type->id);
	}

	void Deserialize(Varlet::Core::DeserializeInfo& info) override
	{
		uint32_t id = info.Read<uint32_t>();
		_type = Varlet::Core::Type::GetType(id);
	}
};