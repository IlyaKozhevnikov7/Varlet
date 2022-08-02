#pragma once

#include "VarletCore.h"
#include <vector>

#define SUCCESSFUL_INITIALIZATION 0
#define FAILED_INITIALIZATION -1

class Scene;

namespace Varlet
{
	class CORE_API Module
	{
	private:

		mutable std::vector<Module*> _dependencies;

	public:

		Module() = default;

		virtual ~Module() = default;

		std::vector<Module*>& GetDependencies() const;

		// for self initialization
		virtual int32_t Init();

		// for initialization with other modules
		virtual int32_t PostInit();

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

	class CORE_API GameModule : public Module, public IUpdatebleModule
	{
	private:

		// startup scene path
		Scene* _currentScene = nullptr;

	public:

		GameModule() = default;

		~GameModule() override = default;

		virtual int32_t Init() override;

		virtual void Update() override;

		virtual void Shutdown() override;
	};
}