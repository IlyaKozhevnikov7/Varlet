#include "ProfilingPanel.h"

#include "EditorCore.h"
#include "VarletFramework.h"

void ProfilingPanel::Update()
{
	ImGui::Begin("Stats");

	if (ImGui::CollapsingHeader("Common", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Delta time: %f ms", Time::GetDeltaTime() * 1000.f);
	}

	if (ImGui::CollapsingHeader("Graphics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//ImGui::Text("Graphics card: %s", GraphicsInfo::rendererName);
		ImGui::Text("FPS: %f", 1.f / Time::GetDeltaTime());
		ImGui::Text("Render time: %.3f ms", GraphicsInfo::renderTime);
	}

	ImGui::End();
}
