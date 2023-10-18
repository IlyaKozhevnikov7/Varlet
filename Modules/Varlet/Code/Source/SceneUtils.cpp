#include "SceneUtils.h"
#include "Scene.h"
#include "Asset.h"
#include "AssetPath.h"
#include <fstream>

#include "Serialization/BinaryFormatter.h"

namespace Varlet
{
	void SceneUtils::Save(const wchar_t* path)
	{
		auto data = BinaryFormatter::Serialize(Scene::_current);

		std::ofstream file(path, std::ios::binary);
		file.write(std::bit_cast<const char*>(data.data()), data.size());
		file.close();
	}

	void SceneUtils::Load(const wchar_t* path)
	{
		std::ifstream stream(path, std::ios::binary);

		stream.seekg(0, std::ios::end);
		std::streampos fileSize = stream.tellg();
		stream.seekg(0, std::ios::beg);

		std::vector<int8_t> data;
		data.resize(fileSize);

		stream.read(std::bit_cast<char*>(data.data()), fileSize);
		stream.close();

		std::vector<SharedResourceInfo> sharedResources;
		Scene* scene = BinaryFormatter::Deserialize<Scene>(data, &sharedResources);

		for (auto& resourceInfo : sharedResources)
		{
			const auto path = AssetPath::DeserializePath(resourceInfo.id);
			IAsset* asset = AssetRegister::Load(path);
		
			if (asset == nullptr)
				continue;
		
			for (auto& dependency : resourceInfo.dependencies)
				*dependency.reference = CastMemory(asset->GetType(), dependency.referenceType, asset->GetObjectTop());
		}

		Scene::Load(scene);
	}
}
