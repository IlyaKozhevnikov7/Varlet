#pragma once

// ========== Utils

#define __GEN_TO_STRING(String) #String

// ========== Aliases

#define __GEN_TYPE_ALIAS(T) using ThisClass = T;
#define __GEN_FIRST_BASE_TYPE_ALIAS(T) using BaseClass = T;

// ========== Get Type Static (Method)

#define __GEN_GET_TYPE_STATIC_METHOD_BEGIN(T) \
	static const Varlet::Core::Type* GetTypeStatic() \
	{ \
		static const auto type = Varlet::Core::Type \
		( \
			__GEN_TO_STRING(T), \
			sizeof(T), \

#define __GEN_GET_TYPE_STATIC_METHOD_PARENTS(...) { __VA_ARGS__ },
#define __GEN_GET_TYPE_STATIC_METHOD_FIELDS_LIST_BEGIN {
#define __GEN_GET_TYPE_STATIC_METHOD_FIELD(Name, T) { __GEN_TO_STRING(Name), __GEN_TO_STRING(T), offsetof(ThisClass, Name), sizeof(ThisClass::Name) },
#define __GEN_GET_TYPE_STATIC_METHOD_FIELDS_LIST_END }, [](){ return reinterpret_cast<void*>(new ThisClass); })

#define __GEN_GET_TYPE_STATIC_METHOD_CREATE_FUNTION 

#define __GEN_GET_TYPE_STATIC_METHOD_ADD_PARENT_INFO(T) + T::GetTypeStatic()
#define __GEN_GET_TYPE_STATIC_METHOD_GENERATION_TYPE_END ;
#define __GEN_GET_TYPE_STATIC_METHOD_END return &type; }

// ========== Get Type (Method)

#define __GEN_GET_TYPE_METHOD \
	virtual const Varlet::Core::Type* GetType() \
	{ \
		return GetTypeStatic(); \
	} \

#define __GEN_GET_TYPE_METHOD_OVERRIDE \
	virtual const Varlet::Core::Type* GetType() override \
	{ \
		return GetTypeStatic(); \
	} \
