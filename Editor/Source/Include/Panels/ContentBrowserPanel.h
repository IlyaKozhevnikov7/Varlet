#pragma once

#include "Panel.h"

#include <filesystem>
#include <unordered_map>

namespace Varlet
{
	class Texture;
}

enum class ContentType : uint8_t
{
	File = 0,
	Folder
};

class ContentBrowserPanel final : public Panel
{
private:

	std::filesystem::path _rootCatalog;
	std::filesystem::path _currentCatalog;

	std::unordered_map<ContentType, std::shared_ptr<Varlet::Texture>> _icons;

public:

	void Init() override;

	void Update() override;

private:

	void DrawLeftSide() const;

	void DrawRightSide();

	void ProccessFolderNode(const std::filesystem::path& path) const;
};

