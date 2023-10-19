#include "ProjectAssetRegister.h"
#include "Asset.h"
#include "ModuleService.h"
#include "Scene.h"
#include "EditorData.h"
#include <fstream>
#include "Serialization/BinaryFormatter.h"

namespace fs = std::filesystem;

std::string ProjectAssetRegister::_directory;
std::set<const Varlet::Core::Type*> ProjectAssetRegister::_assetTypes;
std::unordered_map<std::wstring, AssetInfo> ProjectAssetRegister::_registered;

std::vector<std::wstring> ProjectAssetRegister::_unknownAssets;

constexpr wchar_t assetExtensionString[] = WIDE(".vasset");
constexpr wchar_t sceneExtensionString[] = WIDE(".vscene");

void ProjectAssetRegister::Initialize()
{
	Varlet::Core::ModuleService::OnRegisteredNewModule.Bind(&ProjectAssetRegister::OnRegisteredNewModule);
	Varlet::Core::ModuleService::OnUnregisteredNewModule.Bind(&ProjectAssetRegister::OnUnregisteredNewModule);

	RegisterAssetTypes(Varlet::Core::Type::GetAllTypes());

	const fs::path assetDirectory = EditorData::ProjectPath + WIDE("\\Assets");
	RegisterAllAssets(assetDirectory);
}

void ProjectAssetRegister::Shutdown()
{
	Varlet::Core::ModuleService::OnRegisteredNewModule.UnBind(&ProjectAssetRegister::OnRegisteredNewModule);
	Varlet::Core::ModuleService::OnUnregisteredNewModule.UnBind(&ProjectAssetRegister::OnUnregisteredNewModule);

	Varlet::AssetRegister::UnloadAll();
}

std::set<const Varlet::Core::Type*>& ProjectAssetRegister::GetAssetTypes()
{
	return _assetTypes;
}

AssetInfo* ProjectAssetRegister::GetContentInfo(const std::wstring& path)
{
	if (_registered.contains(path))
		return &_registered[path];
	
	return nullptr;
}

void ProjectAssetRegister::AddAsset(const std::wstring& path, IAsset* asset)
{
	assert(asset);
	assert(_registered.contains(path) == false);

	const auto name = fs::path(path).filename().string();

	_registered[path] = { name, asset->GetType() };

	Varlet::AssetRegister::Rigester(path, asset);
	Save(path, asset);
}

std::vector<const AssetInfo*> ProjectAssetRegister::GetAssetInfoByDirectory(const std::wstring& directory)
{
	std::vector<const AssetInfo*> infos;

	for (auto& asset : _registered)
		if (directory == fs::path(asset.first).parent_path())
			infos.push_back(&asset.second);

	return infos;
}

std::vector<std::wstring_view> ProjectAssetRegister::GetAllAvailableAssetsOf(const Varlet::Core::Type* type)
{
	assert(type);
	assert(type->IsA(IAsset));

	std::vector<std::wstring_view> availablePaths;

	for (auto& available : _registered)
		if (available.second.type == type)
			availablePaths.push_back(available.first);

	return availablePaths;
}

void ProjectAssetRegister::Save(const std::wstring& path, IAsset* asset)
{
	auto data = BinaryFormatter::Serialize(asset);
	
	std::ofstream file(path, std::ios::binary);
	file.write(std::bit_cast<const char*>(data.data()), data.size());
	file.close();
}

void ProjectAssetRegister::RegisterAssetTypes(const std::vector<const Varlet::Core::Type*>& types)
{
	for (auto& type : types)
		if (type->IsA(IAsset) && type->flags.NoHas(Varlet::Core::Abstract))
			_assetTypes.emplace(type);
}

void ProjectAssetRegister::RegisterAllAssets(const std::filesystem::path& from)
{
	for (auto& entry : fs::directory_iterator(from))
	{
		if (entry.is_directory())
		{
			RegisterAllAssets(entry.path());
			continue;
		}

		auto& path = entry.path();
		auto extension = path.extension().wstring();

		const auto baseType = FileExtensionToType(extension);

		if (baseType == nullptr)
			continue;

		char buffer[4];
		std::ifstream file(path, std::ios::binary);
		file.read(buffer, sizeof(buffer));

		const auto type = Varlet::Core::Type::GetType(*std::bit_cast<uint32_t*>(&buffer));
		
		if (type == nullptr)
		{
			_unknownAssets.push_back(path);
			continue;
		}

		const auto name = path.filename().string();

		_registered[path.wstring()] = { name, type };
	}
}

const Varlet::Core::Type* ProjectAssetRegister::FileExtensionToType(const std::wstring& extension)
{
	if (extension == assetExtensionString)
		return IAsset::GetTypeStatic();

	if (extension == sceneExtensionString)
		return Scene::GetTypeStatic();

	return nullptr;
}

void ProjectAssetRegister::OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info)
{
	const auto types = Varlet::Core::Type::GetTypes(info.module);
	RegisterAssetTypes(types);

	RegisterAllAssets(WIDE("W:\\VarletUtils\\Sandbox\\Assets"));
}

void ProjectAssetRegister::OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info)
{
	const auto types = Varlet::Core::Type::GetTypes(info.module);
	
	for (auto type : types)
		_assetTypes.erase(type);
}
