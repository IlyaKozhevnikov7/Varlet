#include "ContentBrowserPanel.h"
#include "EditorCore.h"
#include "VarletFramework.h"

using namespace std::filesystem;

void ContentBrowserPanel::Init()
{
	_rootCatalog = "W:\\Varlet\\Sandbox\\Assets";
	_currentCatalog = _rootCatalog;

	_icons[ContentType::File] = Varlet::Texture::Load("W:\\Varlet\\Editor\\Assets\\document.png");
	_icons[ContentType::Folder] = Varlet::Texture::Load("W:\\Varlet\\Editor\\Assets\\folder.png");
}

void ContentBrowserPanel::Update()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f, 0.1f, 0.1f, 1.f });

	if (ImGui::Begin("Content Browser"))
	{
		ImGui::Columns(2, "Outer", true);
		DrawLeftSide();
		ImGui::NextColumn();
		DrawRightSide();

		ImGui::End();
	}

	ImGui::PopStyleColor();
}

void ContentBrowserPanel::DrawLeftSide() const
{
	if (ImGui::CollapsingHeader(_currentCatalog.string().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow))
		ProccessFolderNode(_rootCatalog);
}

void ContentBrowserPanel::DrawRightSide()
{
	constexpr float iconSize = 60.f;
	constexpr float itemWedth = iconSize * 1.25f;

	int32_t colomnAmount = static_cast<int32_t>(ImGui::GetContentRegionAvail().x / itemWedth);
	colomnAmount = colomnAmount > 0 ? colomnAmount : 1;

	ImGui::Button("Back", { 80.f, 30.f });
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (_currentCatalog != _rootCatalog)
			_currentCatalog = _currentCatalog.parent_path();
	}
	ImGui::SameLine();
	ImGui::Text(_currentCatalog.string().c_str());

	if (ImGui::BeginChild("RightBrowser", ImVec2(0.f, 0.f), true))
	{
		ImGui::Columns(colomnAmount, "Inner", false);

		for (const auto path : directory_iterator(_currentCatalog))
		{
			if (colomnAmount > 1)
				ImGui::SetColumnWidth(-1, itemWedth);

			const std::string itemName = std::filesystem::path(path).filename().string();
			const std::string pathString = std::filesystem::path(path).string();
			const bool isDirectory = path.is_directory();

			ImGui::PushID(itemName.c_str());

			const auto texture = _icons[isDirectory ? ContentType::Folder : ContentType::File];
			ImGui::ImageButton(NATIVE_TEXTURE_TO_IMTEXTUREID(Varlet::RendererAPI::GetNativeTexture(texture)), {iconSize, iconSize}, {0, 1}, {1, 0}, 1, {0.15f, 0.15f, 0.15f, 1.f});

			if (isDirectory == false && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload(TEXTURE_FILE_PAYLOAD, pathString.c_str(), (pathString.size() + 1));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (path.is_directory())
					_currentCatalog /= itemName;
			}

			ImGui::TextWrapped(itemName.c_str());
			ImGui::PopID();
			ImGui::NextColumn();
		}
	}

	ImGui::EndChild();
}

void ContentBrowserPanel::ProccessFolderNode(const std::filesystem::path& path) const
{
	for (const auto path : directory_iterator(path))
		if (path.is_directory() && ImGui::TreeNodeEx(std::filesystem::path(path).filename().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow))
		{
			ProccessFolderNode(path);
			ImGui::TreePop();
		}
}
