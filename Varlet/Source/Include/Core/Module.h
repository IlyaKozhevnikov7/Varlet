#pragma once

#include "Varlet.h"
#include <vector>

namespace Varlet
{
	class VARLET_API Module
	{
	private:

		std::vector<Module*> _dependencies;

	public:

		Module() = default;

		virtual ~Module() = default;

		virtual void Init();

		virtual void InitDependencies(const std::vector<Module*>& dependencies);

		virtual void Shutdown() = 0;

	};

	class IUpdatebleModule
	{
	public:

		virtual ~IUpdatebleModule() = default;

		virtual void Update() = 0;
	};
}