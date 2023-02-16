#pragma once

#include "Common.h"

namespace Varlet::Core
{
	class Type;

	class CORE_API Module
	{
	public:

		Module() = default;

		virtual ~Module() = default;

		virtual std::vector<const char*> GetDependencies() const;

		// for self initialization
		virtual bool Init();

		// for initialization with other modules
		virtual bool PostInit();

		virtual void Shutdown();

		virtual std::vector<const Type*>& GetTypes();
	};

	class IUpdatebleModule
	{
	public:

		virtual ~IUpdatebleModule() = default;

		virtual void Update() = 0;
	};

	class CORE_API EditorModule : public Module, public IUpdatebleModule
	{
	public:

		std::string projectDirectory;

	public:

		EditorModule() = default;

		~EditorModule() override = default;
	};
}