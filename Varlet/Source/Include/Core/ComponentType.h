#pragma once

#include "VarletCore.h"

namespace Varlet
{
	enum class Type : uint8_t
	{
		Bool,
		Int8,
		UInt8,
		Int16,
		UInt16,
		Int32,
		UInt32,
		Int64,
		UInt64,
		Float,
		Double,
		Vector2,
		Vector3,
		Vector4
	};

	struct CORE_API Property final
	{
		const char* name;
		void* value;
		Type type;

		Property(const char* name, bool& value)
			: Property(name, value, Type::Bool)
		{
		}

		Property(const char* name, int8_t& value)
			: Property(name, value, Type::Int8)
		{
		}

		Property(const char* name, uint8_t& value)
			: Property(name, value, Type::UInt8)
		{
		}

		Property(const char* name, int16_t& value)
			: Property(name, value, Type::Int16)
		{
		}

		Property(const char* name, uint16_t& value)
			: Property(name, value, Type::UInt16)
		{
		}

		Property(const char* name, int32_t& value)
			: Property(name, value, Type::Int32)
		{
		}

		Property(const char* name, uint32_t& value)
			: Property(name, value, Type::UInt32)
		{
		}

		Property(const char* name, int64_t& value)
			: Property(name, value, Type::Int64)
		{
		}

		Property(const char* name, uint64_t& value)
			: Property(name, value, Type::UInt64)
		{
		}

		Property(const char* name, float& value)
			: Property(name, value, Type::Float)
		{
		}

		Property(const char* name, double& value)
			: Property(name, value, Type::Double)
		{
		}

		Property(const char* name, glm::vec2& value)
			: Property(name, value.x, Type::Vector2)
		{
		}

		Property(const char* name, glm::vec3& value)
			: Property(name, value.x, Type::Vector3)
		{
		}

		Property(const char* name, glm::vec4& value)
			: Property(name, value.x, Type::Vector4)
		{
		}

	private:

		template<typename T>
		Property(const char* name, T& value, Type&& type)
		{
			this->name = name;
			this->value = reinterpret_cast<void*>(&value);
			this->type = type;
		}
	};

	struct TypeInfo final
	{
		const char* name;
		std::vector<Property> properties;

		TypeInfo(const char* name)
		{
			this->name = name;
		}
	};
}

#define TYPE_GENERATION(Type, BaseType) \
using BaseClass = BaseType; \
public: \
virtual Varlet::TypeInfo GetType(); \
Type() = default; \
~Type() override = default; \

#define EMPTY_PROPERTY_BLOCK(Type) \
Varlet::TypeInfo Type::GetType() \
{ \
auto info = BaseClass::GetType(); \
info.name = #Type; \
return info; \
} \

#define START_PROPERTY_BLOCK(Type) \
Varlet::TypeInfo Type::GetType() \
{ \
auto info = BaseClass::GetType(); \
info.name = #Type; \

#define END_PROPERTY_BLOCK \
return info; \
} \

#define PROPERTY(Name, Value) info.properties.push_back(Varlet::Property(Name, Value));