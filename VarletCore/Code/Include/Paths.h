#pragma once

#include "Common.h"

namespace Varlet::Core
{
	struct Paths;
}

struct CORE_API Paths final
{
	friend struct Varlet::Core::Paths;

private:

	static std::wstring _applicationPath;

#ifdef VARLET_DEVELOPMENT
	static std::wstring _enginePath;

public:

	static const std::wstring& GetEnginePath();

	static std::wstring GetEngineModulePaths();
#endif // VARLET_DEVELOPMENT

public:

	static const std::wstring& GetApplicationPath();
};

namespace Varlet::Core
{
	struct Paths final
	{
	public:

#ifdef VARLET_DEVELOPMENT
		static void InitializeEnginePath(const wchar_t* launcherPath);
#endif // VARLET_DEVELOPMENT

		static void InitializeApplicationPath(const wchar_t* path);
	};
}
