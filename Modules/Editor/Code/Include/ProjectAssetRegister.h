#pragma once

#include "EditorCore.h"
#include "Asset.h"
#include <filesystem>

namespace Varlet::Core
{
	struct ModuleInfo;
}

struct AssetInfo final
{
	std::string name;
	const Varlet::Core::Type* type;
};

class ProjectAssetRegister final
{
private:

	static std::string _directory;
	static std::set<const Varlet::Core::Type*> _assetTypes;
	static std::unordered_map<std::wstring, AssetInfo> _registered;

	static std::vector<std::wstring> _unknownAssets;

public:

	static void Initialize();

	static void Shutdown();

	static std::set<const Varlet::Core::Type*>& GetAssetTypes();

	static AssetInfo* GetContentInfo(const std::wstring& path);

	EDITOR_API static void AddAsset(const std::wstring& path, IAsset* asset);

	static std::vector<const AssetInfo*> GetAssetInfoByDirectory(const std::wstring& directory);

	EDITOR_API static std::vector<std::wstring_view> GetAllAvailableAssetsOf(const Varlet::Core::Type* type);

	template<typename T>
	static std::vector<std::wstring_view> GetAllAvailableAssetsOf()
	{
		static_assert(std::is_base_of_v<IAsset, T>);

		return GetAllAvailableAssetsOf(T::GetTypeStatic());
	}

private:

	static void Save(const std::wstring& path, IAsset* asset);

	static void RegisterAssetTypes(const std::vector<const Varlet::Core::Type*>& types);

	static void RegisterAllAssets(const std::filesystem::path& from);

	static const Varlet::Core::Type* FileExtensionToType(const std::wstring& extension);

	static void OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info);

	static void OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info);

};
