#pragma once

#include "Varlet.h"
#include <vector>

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

		virtual void Init();

		virtual void Shutdown() = 0;

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