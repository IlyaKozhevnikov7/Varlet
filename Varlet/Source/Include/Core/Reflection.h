#pragma once

#include <typeinfo>
#include "VarletCore.h"

class Object;

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
		Vector4,
		BoolVector2,
		BoolVector3,
		BoolVector4,
		Int32Vector2,
		Int32Vector3,
		Int32Vector4,
		UInt32Vector2,
		UInt32Vector3,
		UInt32Vector4,
		DoubleVector2,
		DoubleVector3,
		DoubleVector4,

		Matrix2,
		Matrix3,
		Matrix4,

		Object,
		Array,

		Color3,
		Color4,

		Sampler2D,
		SamplerCube,

		Unknown
	};

#define IS_REFERENCE_TYPE(T) ((T == Type::Object) || (T == Type::Sampler2D) || (T == Type::SamplerCube))

	struct CORE_API Property
	{
		const char* name;
		void* value;
		Type type;

		template<typename T>
		Property(const char* name, T& value)
		{
			const size_t hash = typeid(T).hash_code();

			if (_hashesOfType.contains(hash) == false)
			{
				this->name = nullptr;
				this->value = nullptr;
				type = Type::Unknown;
				return;
			}

			this->name = name;
			this->value = reinterpret_cast<void*>(&value);
			this->type = _hashesOfType[hash];
		}

		template<typename T>
		Property(const char* name, T& value, const Type& type)
		{
			this->name = name;
			this->type = type;
			this->value = &value;
		}

		template<> Property(const char* name, void*& value, const Type& type)
		{
			this->name = name;

			if (IS_REFERENCE_TYPE(type))
			{
				this->value = &value;
				this->type = type;
			}
			else if (value != nullptr)
			{
				this->value = value;
				this->type = type;
			}
			else
			{
				this->value = nullptr;
				this->type = Type::Unknown;
			}
		}

	protected:

		static std::unordered_map<size_t, Type> _hashesOfType;
	};

	struct Array : public Property
	{
		Type contentType;
		int32_t size;

		template<typename T>
		Array(const char* name, std::vector<T>& value)
			: Property(name, value, Type::Array)
		{
			size = value.size();

			if constexpr (std::is_base_of_v<Object, std::remove_pointer_t<T>>)
			{
				contentType = Type::Object;
				return;
			}

			const auto hash = typeid(T).hash_code();

			contentType = _hashesOfType.contains(hash)
				? _hashesOfType[hash]
				: Type::Unknown;
		}
	};

	struct TypeInfo final
	{
		const char* name;
		std::vector<Property*> properties;

		TypeInfo(const char* name);
	};
}

#ifdef META
	#define TYPE_GENERATION(Type, BaseType) \
		using BaseClass = BaseType; \
		public: \
		virtual Varlet::TypeInfo GetType() override; \
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
	
	#define PROPERTY(Name, Value) info.properties.push_back(new Varlet::Property(Name, Value));
	#define TYPED_PROPERTY(Name, Value, Type) info.properties.push_back(new Varlet::Property(Name, Value, Type));
	#define ARRAY(Name, Value) info.properties.push_back(new Varlet::Array(Name, Value)); 
#else
	#define TYPE_GENERATION(Type, BaseType)
	#define EMPTY_PROPERTY_BLOCK(Type)
	#define START_PROPERTY_BLOCK(Type)
	#define END_PROPERTY_BLOCK
	#define PROPERTY(Name, Value)
	#define ARRAY(Name, Value)
#endif // META