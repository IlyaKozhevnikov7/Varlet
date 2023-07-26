#pragma once

#include "Common.h"
#include <typeinfo>
#include <format>
#include "Reflection/Meta.h"

#define REFLECTABLE(...)
#define FIELD(...)
#define ENUM(...)

#define COMBINE(Prefix, Name, Number, Postfix) Prefix##Name##_##Number##Postfix
#define GENERATION_INTERNAL(Prefix, Name, Number, Postfix) COMBINE(Prefix, Name, Number, Postfix)
#define GENERATION GENERATION_INTERNAL(__GEN_, __FILE_ID__, __LINE__, _)

#define GENERATED_ENUM(T) Varlet::Core::EnumType<T>::GetTypeStatic()
#define GENERATED_ARRAY(T, TT, Marks) new Varlet::Core::TemplateArrayType<T>("std::vector<" #T ">", #TT, Marks)

namespace Varlet::Core
{
	class Module;
	class Type;
	struct EnumValue;

	struct CORE_API FieldInfo final
	{
	public:

		std::string name;
		uint32_t typeId;
		size_t offset;	// if field has flag [Static] - offset means location of variable
		size_t size;	// TODO: try delete this
		BitMask<FieldMetaFlags> flags;

	public:

		FieldInfo(const char* name, const char* typeName, size_t offset, size_t size, BitMask<FieldMetaFlags> flags);
	};

	struct CORE_API ParentInfo final
	{
		const Varlet::Core::Type* type;
		size_t vfPtrOffset;
	};

	struct CORE_API Type
	{
		friend struct FieldInfo;

		using CreateFunction = void*(*)();
		using GetActualTypeFunction = const Type*(*)(void*);

	private:

		static std::vector<const Type*> _types;

	public:

		static void Register(const Module* module, const Type* type);

		static void UnregisterModuleTypes(const Module* module);

		static const Type* GetType(const char* name);

		static const Type* GetType(const uint32_t id);

		/*
		* Return 0 if generated type doesnt exist
		*/
		static uint32_t GetTemplateId(const char* name);

		/*
		*  Doesn't return templates that used this module
		*/
		static std::vector<const Type*> GetTypes(const Module* module);

		static const std::vector<const Type*>& GetAllTypes();

		template<typename Predicate>
		static std::vector<const Type*> FindAllTypesByPredicate(Predicate&& predicate)
		{
			std::vector<const Type*> types;

			for (auto type : _types)
				if (predicate(type))
					types.push_back(type);

			return types;
		}

	public:

		std::string name;
		uint32_t id;
		size_t size;
		std::vector<ParentInfo> parentInfos;
		std::vector<FieldInfo> fieldInfos;
		BitMask<TypeMetaFlags> flags;

		const Module* module = nullptr; // null if template

	private:

		GetActualTypeFunction _getActualTypeFunction;
		CreateFunction _createFunction;

	public:

		/*
		 * Class/Struct contructor
		 */
		Type(const char* name, const size_t& size, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, GetActualTypeFunction getActualTypeFunction, CreateFunction createFunction, BitMask<TypeMetaFlags> flags = 0);

		/*
		 * Enum contructor
		 */
		Type(const char* name, const size_t& size, std::initializer_list<EnumValue> values);

		virtual ~Type();

		Type& operator+(const Type* other);

		void* CreateInstance() const;

		const Type* GetActualType(void* value) const;

		const FieldInfo* GetFieldInfo(const char* name) const;

		void* GetFieldData(void* instance, const char* name) const;

		bool CheckIsA(const Type* type) const;

		bool CheckIsA(const uint32_t& typeId) const;

		template<typename T>
		bool CheckIsA() const
		{
			return IsA(T::GetTypeStatic()->id);
		}

		static uint32_t Hash(const char* name);
	};

	template<typename Derived, typename Base>
	size_t VfPtrOffset(const void* buffer)
	{
		static_assert(std::is_base_of_v<Base, Derived>);

		const Derived& obj = (Derived&)buffer;
		const Base& ref = obj;
		return (int8_t*)&ref - (int8_t*)&obj;
	}
}

#define IsA(T) CheckIsA(T::GetTypeStatic())

CORE_API extern void* CastMemory(const Varlet::Core::Type* fromType, const Varlet::Core::Type* toType, int8_t* top);

template<typename To>
To* CastMemory(const Varlet::Core::Type* fromType, int8_t* top)
{
	return (To*)CastMemory(fromType, To::GetTypeStatic(), top);
}

template<typename To, typename From>
To* CastMemory(int8_t* top)
{
	return (To*)CastMemory(From::GetTypeStatic(), To::GetTypeStatic(), top);
}

template<typename To, typename From>
To* Cast(From* obj)
{
	return (To*)CastMemory(obj->GetType(), To::GetTypeStatic(), (int8_t*)obj->GetObjectTop());
}