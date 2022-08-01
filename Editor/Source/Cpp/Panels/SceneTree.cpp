#include "Panels/SceneTree.h"
#include "EditorCore.h"
#include "Scene/Scene.h"

void SceneTree::Init()
{
	// TODO bind to scene changed event
}

void SceneTree::Update()
{
	ImGui::Begin("Scene Tree");
	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	auto all = Scene::GetAll();

	static int32_t selectionMask = (1 << 2);
	int32_t nodeClicked = -1;

	for (int32_t i = 0; i < all.size(); i++)
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth;

		if ((selectionMask & (1 << i)) != 0) // is selected
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx(reinterpret_cast<void*>(i), nodeFlags, "Entity [%i]", i))
		{
			if (ImGui::IsItemClicked())
			{
				nodeClicked = i;
				EditorData::selectedEntity = all[i];
			}

			ImGui::TreePop();
		}
	}

	if (nodeClicked != -1)
		selectionMask = (1 << nodeClicked);

	ImGui::End();
}

void SceneTree::OnSceneChanged(const Varlet::Scene* newScene)
{
	_currnetScene = newScene;
}
