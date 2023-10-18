#include "Paths.h"
#include <filesystem>

namespace fs = std::filesystem;

std::wstring Paths::_applicationPath;

#ifdef VARLET_DEVELOPMENT
std::wstring Paths::_enginePath;

const std::wstring& Paths::GetEnginePath()
{
	return _enginePath;
}

std::wstring Paths::GetEngineModulePaths()
{
	return _enginePath + WIDE("\\Modules");
}
#endif // VARLET_DEVELOPMENT

const std::wstring& Paths::GetApplicationPath()
{
	return _applicationPath;
}

namespace Varlet::Core
{
#ifdef VARLET_DEVELOPMENT
	void Paths::InitializeEnginePath(const wchar_t* launcherPath)
	{
		fs::path path(launcherPath);
		auto parentPath = path.parent_path()
			.parent_path()
			.parent_path()
			.parent_path();

		::Paths::_enginePath = parentPath.wstring();
	}
#endif // VARLET_DEVELOPMENT

	void Paths::InitializeApplicationPath(const wchar_t* path)
	{

	}
}