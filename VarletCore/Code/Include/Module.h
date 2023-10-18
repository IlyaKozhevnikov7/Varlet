#pragma once

#include "Common.h"

namespace Varlet::Core
{
	class Type;

	/*
	 * Base engine managed module witch interprets functional dll
	 */
	class CORE_API Module
	{
	public:

		Module() = default;

		virtual ~Module() = default;

		// for self initialization
		virtual bool Initialize();

		// for initialization with other modules
		virtual bool PostInitialize();

		virtual void Shutdown();
	};

	class IUpdatebleModule
	{
	public:

		virtual ~IUpdatebleModule() = default;

		virtual void Update() = 0;
	};
}