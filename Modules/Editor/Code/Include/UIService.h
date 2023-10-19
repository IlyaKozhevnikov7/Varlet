#pragma once

#include "EditorCore.h"
#include "ImGui/imgui.h"

namespace ImGui
{
	EDITOR_API extern bool TreeNodeDefaultOpen(const char* label, ImGuiTreeNodeFlags flags = 0);
}

class EDITOR_API UIService final
{
public:

	static void DisplayValue(const char* name, const uint32_t& hash, int8_t* ref, BitMask<Varlet::Core::FieldMetaFlags> marks = 0);

	static void DisplayArray(const char* name, const Varlet::Core::IArrayType* arrayType, int8_t* ref);

	static void DisplayTypeOf(const char* name, void* value, const Varlet::Core::TemplateType* type);
};