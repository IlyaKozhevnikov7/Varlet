#include "UIService.h"
#include "Utilities/ImGuiPanelUtility.h"
#include "EditorUtilitySystem.h"

#include "ImGui/imgui_internal.h"

namespace ImGui
{
	bool TreeNodeDefaultOpen(const char* label, ImGuiTreeNodeFlags flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		return ImGui::TreeNodeBehavior(window->GetID(label), flags | ImGuiTreeNodeFlags_DefaultOpen, label);
	}
}

void UIService::DisplayValue(const char* name, const uint32_t& hash, int8_t* ref, BitMask<Varlet::Core::FieldMetaFlags> marks)
{
#define REGISTER_TYPE_DISPLAY_FUNCTION(T, Function) { Varlet::Core::Type::GetType(#T)->id, [](const char* name, void* value) { Function; } }

	constexpr float speed = 0.2f;

	static std::unordered_map<uint32_t, void(*)(const char*, void*)> displayPrimitiveTypeFunctions =
	{
		REGISTER_TYPE_DISPLAY_FUNCTION(bool, ImGui::Checkbox(name, reinterpret_cast<bool*>(value))),
		REGISTER_TYPE_DISPLAY_FUNCTION(int8_t, ImGui::DragScalar(name, ImGuiDataType_S8, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(uint8_t, ImGui::DragScalar(name, ImGuiDataType_U8, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(int16_t, ImGui::DragScalar(name, ImGuiDataType_S16, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(uint16_t, ImGui::DragScalar(name, ImGuiDataType_U16, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(int32_t, ImGui::DragScalar(name, ImGuiDataType_S32, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(uint32_t, ImGui::DragScalar(name, ImGuiDataType_U32, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(int64_t, ImGui::DragScalar(name, ImGuiDataType_S64, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(uint64_t, ImGui::DragScalar(name, ImGuiDataType_U64, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(float, ImGui::DragScalar(name, ImGuiDataType_Float, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(double, ImGui::DragScalar(name, ImGuiDataType_Double, value, speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(glm::vec3, ImGui::DragFloat3(name, reinterpret_cast<float*>(value), speed)),
		REGISTER_TYPE_DISPLAY_FUNCTION(Color, ImGui::ColorEdit4(name, reinterpret_cast<float*>(value), ImGuiColorEditFlags_NoInputs)),
	};

	static std::unordered_map<uint32_t, void(*)(const char*, void*, const Varlet::Core::TemplateType*)> displaySpecialGeneratedTypeFunctions =
	{
		{ Varlet::Core::Type::GetTemplateId("TypeOf"), &UIService::DisplayTypeOf }
	};

	auto type = Varlet::Core::Type::GetType(hash);
	if (type == nullptr)
		return;

	if (type->flags.Has(Varlet::Core::GeneratedType))
	{
		auto generatedType = std::bit_cast<Varlet::Core::TemplateType*>(type);

		if (displaySpecialGeneratedTypeFunctions.contains(generatedType->templateId))
		{
			displaySpecialGeneratedTypeFunctions[generatedType->templateId](name, ref, generatedType);
			return;
		}

		if (auto templateArray = dynamic_cast<const Varlet::Core::IArrayType*>(type))
		{
			DisplayArray(name, templateArray, ref);
			return;
		}
	}
	else if (displayPrimitiveTypeFunctions.contains(hash))
	{
		displayPrimitiveTypeFunctions[hash](name, ref);
		return;
	}

	const bool isPointer = marks.Has(Varlet::Core::Pointer);

	void* data = isPointer
		? *std::bit_cast<void**>(ref)
		: ref;

	static ImGuiPanelUtility* inlinePanelUtility = EditorUtilitySystem::FindByType<ImGuiPanelUtility>();
	assert(inlinePanelUtility);

	inlinePanelUtility->DisplayProperty(type, name, ref, isPointer);
}

void UIService::DisplayArray(const char* name, const Varlet::Core::IArrayType* arrayType, int8_t* ref)
{
	const size_t size = arrayType->Size(ref);
	const auto tType = Varlet::Core::Type::GetType(arrayType->tId);
	const int32_t elementSize = arrayType->tFlags.Has(Varlet::Core::Pointer)
		? sizeof(size_t)
		: tType->size;

	if (tType == nullptr)
	{
		// TODO: log does not support type
		return;
	}

	if (ImGui::TreeNode(name))
	{
		for (int32_t i = 0; i < size; i++)
		{
			const std::string label = std::format("Element {}", i);

			int8_t* value = arrayType->GetData(ref) + i * elementSize;

			ImGui::PushID(i);

			UIService::DisplayValue(label.c_str(), arrayType->tId, value, arrayType->tFlags);

			if (ImGui::BeginPopupContextItem(label.c_str()))
			{
				if (ImGui::Selectable("Delete element"))
					arrayType->Delete(ref, i);

				ImGui::EndPopup();
			}

			ImGui::PopID();
		}

		if (ImGui::Button("Add element"))
			arrayType->Add(ref);

		ImGui::TreePop();
	}
}

void UIService::DisplayTypeOf(const char* name, void* value, const Varlet::Core::TemplateType* type)
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
