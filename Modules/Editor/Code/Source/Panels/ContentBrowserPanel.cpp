#include "ContentBrowserPanel.h"
#include "ProjectAssetRegister.h"
#include "EditorData.h"

#include "EditorUtilitySystem.h"
#include "Utilities/ImGuiPanelUtility.h"

#include <locale>
#include <codecvt>

namespace fs = std::filesystem;

void ContentBrowserPanel::Initialize()
{
	_rootCatalog = EditorData::ProjectPath + WIDE("\\Assets");
	_currentDirectory = _rootCatalog;

	OnChangedDirectory();

	_panelUtility = EditorUtilitySystem::FindByType<ImGuiPanelUtility>();
	assert(_panelUtility);
}

void ContentBrowserPanel::Update()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f, 0.1f, 0.1f, 1.f });

	ImGui::Begin("Content Browser");
	
	ImGui::Columns(2, "Outer", true);
	DrawLeftSide();
	ImGui::NextColumn();
	DrawRightSide();

	ImGui::End();

	ImGui::PopStyleColor();
}

void ContentBrowserPanel::DrawLeftSide() const
{
	if (ImGui::CollapsingHeader(_currentDirectory.string().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow))
		ProccessFolderNode(_rootCatalog);
}

void ContentBrowserPanel::DrawRightSide()
{
	constexpr float iconSize = 60.f;
	constexpr float itemWedth = iconSize * 1.25f;

	int32_t colomnAmount = static_cast<int32_t>(ImGui::GetContentRegionAvail().x / itemWedth);
	colomnAmount = colomnAmount > 0 ? colomnAmount : 1;

	if (ImGui::Button("Back", { 80.f, 30.f }))
	{
		if (_currentDirectory != _rootCatalog)
			_currentDirectory = _currentDirectory.parent_path();

		_needChangeDirectoryEvent = true;
	}

	ImGui::SameLine(100.f);
	if (ImGui::Button("Add Asset", { 80.f, 30.f }))
		ImGui::OpenPopup("AddAssetPopup");

	if (ImGui::BeginPopup("AddAssetPopup"))
	{
		for (auto& type : ProjectAssetRegister::GetAssetTypes())
			if (ImGui::Selectable(type->name.c_str()))
			{
				_panelUtility->OpenInitializationAssetPanel(type, _currentDirectory.wstring(), _needChangeDirectoryEvent);
				break;
			}

		ImGui::EndPopup();
	}

	ImGui::SameLine();
	ImGui::Text(_currentDirectory.string().c_str());

	if (ImGui::BeginChild("RightBrowser", ImVec2(0.f, 0.f), true))
	{
		ImGui::Columns(colomnAmount, "Inner", false);

		for (const auto path : fs::directory_iterator(_currentDirectory))
		{
			if (colomnAmount > 1)
				ImGui::SetColumnWidth(-1, itemWedth);
		
			if (path.is_directory() == false)
				continue;
		
			const std::string itemName = std::filesystem::path(path).filename().string();
		
			ImGui::PushID(itemName.c_str());
			
			const auto texture = _icons[ContentType::Folder];
			ImGui::Button("Folder", { iconSize, iconSize });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				_currentDirectory /= itemName;
				_needChangeDirectoryEvent = true;
			}
		
			ImGui::TextWrapped(itemName.c_str());
			ImGui::PopID();
			ImGui::NextColumn();
		}

		for (auto info : _currentDirectoryAssetInfos)
		{
			assert(info);

			if (colomnAmount > 1)
				ImGui::SetColumnWidth(-1, itemWedth);
		
			ImGui::PushID(info);
		
			const auto texture = _icons[ContentType::File];
			const std::wstring path = (_currentDirectory / info->name).wstring();

			ImGui::Button("Asset", { iconSize, iconSize });

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload(info->type->name.c_str(), path.data(), path.size() * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (auto panelType = _panelUtility->GetContextPanelFor(info->type))
				{
					IAsset* asset = Varlet::AssetRegister::Load(path);
					
					if (asset != nullptr)
						_panelUtility->OpenContextPanel(panelType, info->type->name.c_str(), asset);
				}
			}

			ImGui::TextWrapped(info->name.c_str());
			ImGui::PopID();
			ImGui::NextColumn();
		}
	}

	ImGui::EndChild();

	if (_needChangeDirectoryEvent)
		OnChangedDirectory();
}

void ContentBrowserPanel::ProccessFolderNode(const std::filesystem::path& path) const
{
	for (const auto path : fs::directory_iterator(path))
		if (path.is_directory() && ImGui::TreeNodeEx(std::filesystem::path(path).filename().string().c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow))
		{
			ProccessFolderNode(path);
			ImGui::TreePop();
		}
}

void ContentBrowserPanel::OnChangedDirectory()
{
	_currentDirectoryAssetInfos.clear();
	_currentDirectoryAssetInfos = ProjectAssetRegister::GetAssetInfoByDirectory(_currentDirectory.wstring());

	_needChangeDirectoryEvent = false;
}
