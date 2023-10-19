#include "PropertyPanel.h"
#include "EditorData.h"
#include "UIService.h"

void PropertyPanel::Update()
{
	ImGui::Begin("Property");

	const auto selected = EditorData::SelectedEntity;
	
	if (selected != nullptr && EditorData::IsReloading == false)
	{
		const auto components = selected->GetComponents();
		for (auto component : components)
		{
			auto type = component->GetType();
	
			if (type == nullptr)
				continue;
	
			ImGui::PushID(component);
	
			if (ImGui::CollapsingHeader(type->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{	
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
					ImGui::OpenPopup(type->name.c_str());

				int8_t* top = component->GetObjectTop();

				for (const auto& field : type->fieldInfos)
				{
					if (field.flags.NoHas(Varlet::Core::EditorAccessible))
						continue;
	
					int8_t* location = field.flags.Has(Varlet::Core::Static)
						? 0
						: top;
	
					int8_t* ref = location + field.offset;

					UIService::DisplayValue(field.name.c_str(), field.typeId, ref, field.flags);
				}
				
				if (ImGui::BeginPopupContextItem(type->name.c_str()))
				{
					if (ImGui::Selectable("Delete component"))
						selected->RemoveComponent(component);

					ImGui::EndPopup();
				}
			}
	
			ImGui::PopID();
		};
	
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponentPopup");
	
		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			for (auto& type : EditorData::ContextComponentTypes)
				if (ImGui::Selectable(type->name.c_str()))
					selected->AddComponent(type);
	
			ImGui::EndPopup();
		}
	}

	ImGui::End();
}

void PropertyPanel::DisplayTypeOf(const char* name, void* value, const Varlet::Core::TemplateType* type)
{
	auto current = std::bit_cast<const Varlet::Core::Type**>(type->GetFieldData(value, NAME_OF(_type)));

	std::vector<const Varlet::Core::Type*> available;
	std::vector<const char*> names;

	auto typeOf = Varlet::Core::Type::GetType(type->parameterList[0].id);
	for (auto selected : Varlet::Core::Type::GetAllTypes())
		if (selected->CheckIsA(typeOf))
		{
			available.push_back(selected);
			names.push_back(selected->name.c_str());
		}

	available.push_back(nullptr);
	names.push_back("None");

	auto it = std::find(available.begin(), available.end(), *current);
	int32_t currentIndex = it - available.begin();
	int32_t selectedIndex = currentIndex;

	if (ImGui::Combo(name, &selectedIndex, names.data(), names.size())
		&& selectedIndex != currentIndex)
	{
		*current = available[selectedIndex];
	}
}
