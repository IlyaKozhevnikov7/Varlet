#pragma once

#include "Panels/ImGuiPanel.h"
#include "VarletFramework.h"
#include <functional>
#include "PropertyPanel.generated.h"

REFLECTABLE()
class PropertyPanel final : public ImGuiPanel
{
	GENERATION

	using DisplayPrimitiveTypeFunction = void(*)(const char*, void*);

private:

	std::unordered_map<uint32_t, DisplayPrimitiveTypeFunction> _displayPrimitiveTypeFunctions;
	std::unordered_map<uint32_t, std::function<void(const char*, void*)>> _displaySpecialTypeFunctions;
	std::unordered_map<uint32_t, std::function<void(const char*, void*, const Varlet::Core::TemplateType*)>> _displaySpecialGeneratedTypeFunctions;

public:

	void Update() override;

private:

	void DisplayTypeOf(const char* name, void* value, const Varlet::Core::TemplateType* type);
};