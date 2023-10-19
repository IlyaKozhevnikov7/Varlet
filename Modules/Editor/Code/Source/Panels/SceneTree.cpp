#include "Panels/SceneTree.h"
#include "Scene.h"
#include "EditorData.h"
#include "Entity.h"

void SceneTree::Initialize()
{
	BaseClass::Initialize();

	// TODO: Bind to scene changed event
}

void SceneTree::Update()
{
	ImGui::Begin("Scene Tree");
	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	auto entities = Scene::GetAllEntities();

	static int32_t selectionMask = (1 << 2);
	int32_t nodeClicked = -1;

	for (int32_t i = 0; i < entities.size(); i++)
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth;

		if ((selectionMask & (1 << i)) != 0) // NOTE: is selected
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx(reinterpret_cast<void*>(i), nodeFlags, "Entity [%i]", i))
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				nodeClicked = i;
				EditorData::SelectedEntity = entities[i];
			}

			if (ImGui::BeginPopupContextItem((char*)entities[i]))
			{
				if (ImGui::Selectable("Delete"))
				{
					if (EditorData::SelectedEntity == entities[i])
						EditorData::SelectedEntity = nullptr;

					Scene::Destroy(entities[i]);
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload(Entity::GetTypeStatic()->name.c_str(), &entities[i], sizeof(&entities[i]));
				ImGui::EndDragDropSource();
			}

			ImGui::TreePop();
		}
	}

	if (nodeClicked != -1)
		selectionMask = (1 << nodeClicked);

	ImGui::End();
}

void SceneTree::OnSceneChanged(const Scene* newScene)
{
	_currnetScene = newScene;
}
