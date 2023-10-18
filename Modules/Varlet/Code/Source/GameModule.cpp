#include "GameModule.h"
#include "ModuleService.h"
#include "Asset.h"
#include <filesystem>

namespace Varlet
{
	bool Module::PostInitialize()
	{
		const std::wstring libraryPath = Core::ModuleService::GetModulePath(this);
		const std::wstring assetsDirectory = ConvertLibraryPathToAssetDirectoryPath(libraryPath);
		
		RegisterAllAssets(assetsDirectory);

		return true;
	}

	std::wstring Module::ConvertLibraryPathToAssetDirectoryPath(const std::wstring& pathString)
	{
		std::filesystem::path path(pathString);

		auto projectDirectory = path.parent_path()
			.parent_path()
			.parent_path()
			.parent_path();

		return (projectDirectory / "Assets").wstring();
	}

	void Module::RegisterAllAssets(const std::wstring& from)
	{
		for (auto entry : std::filesystem::directory_iterator(from))
		{
			const std::wstring pathAsString = entry.path().wstring();

			if (entry.is_directory())
				RegisterAllAssets(pathAsString);

			if (entry.path().extension() != ".vasset")
				continue;

			AssetRegister::Load(pathAsString);
		}
	}
}
