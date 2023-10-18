#include "Asset.h"
#include "AssetPath.h"
#include "Serialization/BinaryFormatter.h"
#include <filesystem>
#include <fstream>

std::vector<int8_t> IAsset::GetSharedId()
{
	const auto path = Varlet::AssetRegister::GetPathOf(this);

	if (path == WIDE(""))
		return {};

	return AssetPath::SerializePath(path);
}

namespace Varlet
{
	std::unordered_map<std::wstring, IAsset*> AssetRegister::_registered;

	IAsset* AssetRegister::Load(const std::wstring& path)
	{
		if (_registered.contains(path))
			return _registered[path];

		if (std::filesystem::exists(path) == false)
		{
			VARLET_LOG(Warning, WIDE("File does not exist %s"), path.c_str());
			return nullptr;
		}

		return LoadInternal(path);
	}

	void AssetRegister::Rigester(const std::wstring& path, IAsset* asset)
	{
		assert(asset);
		assert(path.empty() == false);

		if (_registered.contains(path))
		{
			VT_LOG(Error, "Asset was registered twice %s", path.c_str())
			return;
		}

		_registered[path] = asset;
	}

	const std::wstring& AssetRegister::GetPathOf(const IAsset* asset)
	{
		for (auto& entry : _registered)
			if (entry.second == asset)
				return entry.first;

		return {};
	}

	void AssetRegister::Unload(IAsset* asset)
	{
		UnloadInternal(asset, true);
	}

	void AssetRegister::Save(const std::wstring& path, IAsset* asset)
	{
		auto data = BinaryFormatter::Serialize(asset);

		std::ofstream file(path, std::ios::binary);
		file.write(std::bit_cast<const char*>(data.data()), data.size());
		file.close();
	}

	void AssetRegister::UnloadAll()
	{
		for (auto& entry : _registered)
			UnloadInternal(entry.second, false);
		
		for (auto& entry : _registered)
			delete entry.second;
	}

	IAsset* AssetRegister::LoadInternal(const std::wstring& path)
	{
		std::ifstream stream(path, std::ios::binary);

		stream.seekg(0, std::ios::end);
		std::streampos fileSize = stream.tellg();
		stream.seekg(0, std::ios::beg);

		std::vector<int8_t> data;
		data.resize(fileSize);

		stream.read(std::bit_cast<char*>(data.data()), fileSize);
		stream.close();

		IAsset* asset = BinaryFormatter::Deserialize<IAsset>(data);
		
		if (asset != nullptr)
		{
			Rigester(path, asset);
			VARLET_LOG(Normal, WIDE("AssetRegister: Asset has been loaded %s"), path.c_str());
			return asset;
		}

		VARLET_LOG(Error, WIDE("AssetRegister: Failed to load %s"), path.c_str());
		return nullptr;
	}

	void AssetRegister::UnloadInternal(IAsset* asset, bool shouldDelete)
	{
		for (auto& entry : _registered)
			if (entry.second == asset)
			{
				Save(entry.first, asset);
				VARLET_LOG(Normal, WIDE("AssetRegister: Asset has been unloaded %s"), entry.first.c_str());

				if (shouldDelete)
					delete asset;

				return;
			}
	}
}
