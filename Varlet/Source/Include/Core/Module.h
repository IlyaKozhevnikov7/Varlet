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

		Module() = default;

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

	class Scene;

	class CORE_API GameModule : public Module, public IUpdatebleModule
	{
	private:

		// startup scene path
		Scene* _currentScene = nullptr;

	public:

		GameModule() = default;

		~GameModule() override = default;

		Scene* GetCurrentScene() const;	

		virtual int32_t Init() override;

		virtual void Update() override;
	};
}