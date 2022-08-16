#include "ComponentType.h"

namespace Varlet
{
	std::unordered_map<size_t, Type> Property::_hashesOfType =
	{
		{ typeid(bool).hash_code(),			Type::Bool },
		{ typeid(int8_t).hash_code(),		Type::Int8 },
		{ typeid(uint8_t).hash_code(),		Type::UInt8 },
		{ typeid(int16_t).hash_code(),		Type::Int16 },
		{ typeid(uint16_t).hash_code(),		Type::UInt16 },
		{ typeid(int32_t).hash_code(),		Type::Int32 },
		{ typeid(uint32_t).hash_code(),		Type::UInt32 },
		{ typeid(int64_t).hash_code(),		Type::Int64 },
		{ typeid(uint64_t).hash_code(),		Type::UInt64 },
		{ typeid(float).hash_code(),		Type::Float },
		{ typeid(double).hash_code(),		Type::Double },

		{ typeid(glm::vec2).hash_code(),	Type::Vector2 },
		{ typeid(glm::vec3).hash_code(),	Type::Vector3 },
		{ typeid(glm::vec4).hash_code(),	Type::Vector4 },
		
		{ typeid(glm::bvec2).hash_code(),	Type::BoolVector2 },
		{ typeid(glm::bvec3).hash_code(),	Type::BoolVector3 },
		{ typeid(glm::bvec4).hash_code(),	Type::BoolVector4 },

		{ typeid(glm::ivec2).hash_code(),	Type::Int32Vector2 },
		{ typeid(glm::ivec3).hash_code(),	Type::Int32Vector3 },
		{ typeid(glm::ivec4).hash_code(),	Type::Int32Vector4 },

		{ typeid(glm::uvec2).hash_code(),	Type::UInt32Vector2 },
		{ typeid(glm::uvec3).hash_code(),	Type::UInt32Vector3 },
		{ typeid(glm::uvec4).hash_code(),	Type::UInt32Vector4 },

		{ typeid(glm::dvec2).hash_code(),	Type::DoubleVector2 },
		{ typeid(glm::dvec3).hash_code(),	Type::DoubleVector3 },
		{ typeid(glm::dvec4).hash_code(),	Type::DoubleVector4 },
	};

	Property::Property(const char* name, Object* value)
	{
		assert(value != nullptr && "Value must to be not nullptr");

		this->name = name;
		this->value = value;
		type = Type::Object;
	}

	Property::Property(const char* name, void* value, const Type& type)
	{
		assert(value != nullptr && "Value must to be not nullptr");

		this->name = name;
		this->value = value;
		this->type = type;
	}

	TypeInfo::TypeInfo(const char* name)
	{
		this->name = name;
	}
}