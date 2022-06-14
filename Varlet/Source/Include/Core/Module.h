#pragma once

#include "VarletCore.h"
#include <vector>

#define SUCCESSFUL_INITIALIZATION 0
#define FAILED_INITIALIZATION -1

namespace Varlet
{
	class CORE_API Module
	{
	private:

		mutable std::vector<Module*> _dependencies;

	public:

		Module();

		virtual ~Module() = default;

		std::vector<Module*>& GetDependencies() const;

		virtual int32_t Init();

		virtual void Shutdown();

	protected:

		virtual void InitDependencies(std::vector<Module*>& dependencies);

	};

	class IUpdatebleModule
	{
	public:

		virtual ~IUpdatebleModule() = default;

		virtual void Update() = 0;
	};
}