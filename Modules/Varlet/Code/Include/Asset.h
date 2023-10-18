#pragma once

#include "VarletCore.h"
#include "Asset.generated.h"

namespace Varlet::Core
{
	class EditorModule;
}

REFLECTABLE(Interface)
class VARLET_API IAsset : public ISharedSerializable
{
	GENERATION

	friend class Asset;

public:

	virtual ~IAsset() = default;

	IAsset() = default;

	std::vector<int8_t> GetSharedId() override;
};

namespace Varlet
{
	class VARLET_API AssetRegister final
	{
	private:

		static std::unordered_map<std::wstring, IAsset*> _registered;

	public:

		static IAsset* Load(const std::wstring& path);

		static void Rigester(const std::wstring& path, IAsset* asset);

		static const std::wstring& GetPathOf(const IAsset* asset);

		static void Unload(IAsset* asset);

		static void Save(const std::wstring& path, IAsset* asset);

		static void UnloadAll();

	private:

		static IAsset* LoadInternal(const std::wstring& path);

		static void UnloadInternal(IAsset* asset, bool shouldDelete);
	};
}
