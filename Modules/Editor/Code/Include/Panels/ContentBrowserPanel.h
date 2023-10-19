#pragma once

#include "Panels/ImGuiPanel.h"

#include <filesystem>
#include <unordered_map>
#include "ContentBrowserPanel.generated.h"

class Texture2D;
class IAsset;
struct AssetInfo;

namespace Varlet::Core
{
	class Type;
	struct ModuleInfo;
}

enum class ContentType : uint8_t
{
	File = 0,
	Folder
};

REFLECTABLE()
class ContentBrowserPanel final : public ImGuiPanel
{
	GENERATION

private:

	std::filesystem::path _rootCatalog;
	std::filesystem::path _currentDirectory;

	std::vector<const AssetInfo*> _currentDirectoryAssetInfos;

	std::unordered_map<ContentType, Texture2D*> _icons;

	bool _needChangeDirectoryEvent;

	ImGuiPanelUtility* _panelUtility;

public:

	void Initialize() override;

	void Update() override;

private:

	void DrawLeftSide() const;

	void DrawRightSide();

	void ProccessFolderNode(const std::filesystem::path& path) const;

	void OnChangedDirectory();
};

