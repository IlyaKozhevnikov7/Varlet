#pragma once

#include "Common.h"
#include "Module.h"
#include <typeinfo>

#include "GenerationMacros.h"

#define SERIALIZABLE(...)

#define COMBINE(Prefix, Name, Number, Postfix) Prefix##Name##_##Number##Postfix
#define GENERATION_INTERNAL(Prefix, Name, Number, Postfix) COMBINE(Prefix, Name, Number, Postfix)
#define GENERATION GENERATION_INTERNAL(__GEN_, __FILE_ID__, __LINE__, _)

#define STATIC_CALL(T, Method, ...) (reinterpret_cast<T*>(0)->Method(__VA_ARGS__))
#define FORCE_CAST_EMPTY(From, To) static_cast<To*>(reinterpret_cast<From*>(0))

namespace Varlet::Core
{
	struct CORE_API FieldInfo final
	{
		const char* name;
		uint32_t typeId;
		size_t offset;
		size_t size;

		FieldInfo(const char* name, const char* typeName, const size_t& offset, const size_t& size);
	};

	struct CORE_API Type final
	{
		friend struct FieldInfo;

		using CreateFunction = void*(*)();

	private:

		static std::unordered_map<const Module*, std::vector<const Type*>> _types;

	public:

		static void Register(const Module* module, const Type* type);

		static void UnregisterModuleTypes(const Module* module);

		static const Type* GetType(const char* name);

		static std::vector<const Type*>& GetTypes(const Module* module);

		static std::vector<const Type*> GetAllTypes();

	public:

		const char* name;
		uint32_t id;
		size_t size;
		std::vector<const char*> parentsNames;
		std::vector<FieldInfo> fieldInfos;

	private:

		CreateFunction _createFunction;

	public:

		Type(const char* name, const size_t& size, std::initializer_list<const char*> parentsNames, std::initializer_list<FieldInfo> fieldInfos, CreateFunction createFunction);

		Type& operator+(const Type* other);

		void* CreateInstance() const;

		bool IsA(const char* typeName) const;

		bool IsA(const uint32_t& typeId) const;

	private:

		static uint32_t Hash(const char* name);
	};
}
