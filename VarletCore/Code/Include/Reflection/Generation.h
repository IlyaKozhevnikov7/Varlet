#pragma once

namespace Varlet::Core
{
	template<typename T>
	struct TypeName
	{
		constexpr static char name[] = "";
	};

	template<typename T>
	struct GeneratedTypeParameners
	{
		static std::vector<struct TemaplateParameter> Get()
		{
			return { };
		}
	};

	class Type;

	template<typename T>
	struct EnumType
	{
		static const Type* GetTypeStatic()
		{
			return nullptr;
		}
	};
}

// ========== Utils

#define __GEN_TO_STRING(String) #String
#define __GEN_COMPRESS(F, String, L) String

#define __GEN_VIRTUAL_MODIFICATION virtual
#define __GEN_EMPTY_MODIFICATION

// ========== Aliases

#define __GEN_TYPE_ALIAS(T) using ThisClass = T;
#define __GEN_FIRST_BASE_TYPE_ALIAS(T) using BaseClass = T;

// ========== Default deneration

#define __GEN_DEFAULT \
	//template<typename Base, typename Derived> \
	//friend size_t Varlet::Core::VfPtrOffset(const void*); \
	//template<typename To, typename From> \
	//friend To Cast(From); \
	//template<typename To, typename From> \
	//friend To* Cast(From*); \

// TODO: virtual destructor


// ========== Create Method

#define __GEN_CREATE_METHOD_BODY_DEFAULT return std::bit_cast<void*>(new ThisClass)
#define __GEN_CREATE_METHOD_BODY_ABSTRACT return nullptr

#define __GEN_CREATE_METHOD(Body) \
	static void* __GEN_Create() \
	{ \
		Body; \
	} \

#define __GEN_GET_ACTUAL_TYPE_METHOD \
	static const Varlet::Core::Type* __GEN_GetActualType(void* value) \
	{ \
		return std::bit_cast<ThisClass*>(value)->GetType(); \
	} \

// ========== Get Type Static (Method)

#define __GEN_GET_TYPE_STATIC_METHOD_BEGIN(T) \
	static const Varlet::Core::Type* GetTypeStatic() \
	{ \
		static const auto type = Varlet::Core::Type \
		( \
			Varlet::Core::TypeName<ThisClass>::name, \
			sizeof(ThisClass), \

#define __GEN_GET_TYPE_STATIC_METHOD_BEGIN_TEMPLATE(T) \
	static const Varlet::Core::Type* GetTypeStatic() \
	{ \
		static const auto type = Varlet::Core::TemplateType \
		( \
			Varlet::Core::TypeName<ThisClass>::name, \
			__GEN_TO_STRING(T), \
			sizeof(ThisClass), \
			Varlet::Core::GeneratedTypeParameners<ThisClass>::Get(), \

#define __GEN_PARENT_INFO(Parent) { Parent::GetTypeStatic(), Varlet::Core::VfPtrOffset<ThisClass, Parent>(Varlet::Core::TypeName<ThisClass>::name) }

#define __GEN_GET_TYPE_STATIC_METHOD_PARENTS(...) { __VA_ARGS__ },
#define __GEN_GET_TYPE_STATIC_METHOD_FIELDS_LIST_BEGIN {

#define __GEN_FIELD_ADDRESS(Mathod, Name) Mathod(Name)
#define __GEN_FIELD_LOCAL(Name) offsetof(ThisClass, Name)
#define __GEN_FIELD_GLOBAL(Name) std::bit_cast<size_t>(&ThisClass::Name)
#define __GEN_GET_TYPE_STATIC_METHOD_FIELD(Attributes, Location, Name, ...) { __GEN_TO_STRING(Name), Varlet::Core::TypeName<__VA_ARGS__>::name, __GEN_FIELD_ADDRESS(Location, Name), sizeof(ThisClass::Name), Attributes },

#define __GEN_GET_TYPE_STATIC_METHOD_FIELD_EXCEPTION(Attributes, Location, Name, ...) { __GEN_TO_STRING(Name), __GEN_TO_STRING(__VA_ARGS__), __GEN_FIELD_ADDRESS(Location, Name), sizeof(ThisClass::Name), Attributes },

#define __GEN_GET_TYPE_STATIC_METHOD_FIELDS_LIST_END }, &ThisClass::__GEN_GetActualType, &ThisClass::__GEN_Create
#define __GEN_GET_TYPE_STATIC_METHOD_META(Value) , Value
#define __GEN_GET_TYPE_STATIC_METHOD_CONSTRUCTOR_END )

#define __GEN_GET_TYPE_STATIC_METHOD_ADD_PARENT_INFO(T) + T::GetTypeStatic()
#define __GEN_GET_TYPE_STATIC_METHOD_GENERATION_TYPE_END ;
#define __GEN_GET_TYPE_STATIC_METHOD_END return &type; }

// ========== Get Type (Method)

#define __GEN_GET_TYPE_METHOD(VirtualModificator) \
	VirtualModificator const Varlet::Core::Type* GetType() const \
	{ \
		return GetTypeStatic(); \
	} \

// ========== Cast To Actual Type (Method)

#define __GEN_CAST_TO_ACTUAL_TYPE(VirtualModificator) \
	VirtualModificator int8_t* GetObjectTop() \
	{ \
		return (int8_t*)(ThisClass*)(this); \
	} \

// ========== Templates ==========

#define __GEN_REGISTER_TYPE_NAME_INTERNAL(Type) \
	namespace Varlet::Core \
	{ \
		template<> \
		struct TypeName<Type> \
		{ \
			constexpr static char name[] = __GEN_TO_STRING(Type); \
		}; \
	} \

#define __GEN_REGISTER_GENERATED_TYPE_PARAMETERS(Type, ...) \
	namespace Varlet::Core \
	{ \
		template<> \
		struct GeneratedTypeParameners<Type> \
		{ \
			static std::vector<TemaplateParameter> Get() \
			{ \
				return { __VA_ARGS__ }; \
			} \
		}; \
	} \

//  ========== Enum ==========

#define __GEN_ENUM_TYPE_BEGIN(T) \
	namespace Varlet::Core \
	{ \
		template<> \
		struct EnumType<T> \
		{ \
		private: \
		__GEN_TYPE_ALIAS(T) \
		public: \
			static const Type* GetTypeStatic() \
			{ \
				static const auto type = Varlet::Core::Type \
				( \
					__GEN_TO_STRING(T), \
					sizeof(ThisClass), \
					{ \

#define __GEN_ENUM_VALUE(Value) { __GEN_TO_STRING(Value), (size_t)ThisClass::Value },

#define __GEN_ENUM_TYPE_END \
		}); \
		return &type; \
		} \
	}; \
	} \

#define __GEN_REGISTER_TYPE_NAME(TypeOf, Type) \
	TypeOf Type; \
	__GEN_REGISTER_TYPE_NAME_INTERNAL(Type) \

#define __GEN_REGISTER_GENERATED_TYPE_INFO(Type, ...) \
	__GEN_REGISTER_TYPE_NAME_INTERNAL(Type) \
	__GEN_REGISTER_GENERATED_TYPE_PARAMETERS(Type, __VA_ARGS__) \

#define __GEN_TEMPLATE_FORWARD_DECLARATION(TypeOf, TemaplateName, ...) \
	template<__VA_ARGS__> \
	TypeOf TemaplateName; \

#define __GEN_ENUM_VALUE_NAME(Type, Value) { __GEN_TO_STRING(Value), (size_t)Type::Value }

#define __GEN_REGISTER_ENUM_INFO(Type, ...) \
	namespace Varlet::Core \
	{ \
		template<> \
		struct GeneratedEnumValues<Type> \
		{ \
			const static size_t size = sizeof(Type); \
			static std::vector<EnumValue> Get() \
			{ \
				return { __VA_ARGS__ }; \
			} \
		}; \
	} \

__GEN_REGISTER_TYPE_NAME_INTERNAL(bool);
__GEN_REGISTER_TYPE_NAME_INTERNAL(int8_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(uint8_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(int16_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(uint16_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(int32_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(uint32_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(int64_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(uint64_t);
__GEN_REGISTER_TYPE_NAME_INTERNAL(float);
__GEN_REGISTER_TYPE_NAME_INTERNAL(double);
__GEN_REGISTER_TYPE_NAME_INTERNAL(std::string);