#include "PropertyPanel.h"

#include "EditorCore.h"
#include "VarletFramework.h"

void PropertyPanel::Update()
{
	ImGui::Begin("Property");

	if (EditorData::selectedEntity == nullptr)
	{
		ImGui::End();
		return;
	}
	
	const auto components = EditorData::selectedEntity->GetComponents();
	for (auto compoent : components)
	{
		ImGui::PushID(compoent);

		auto type = compoent->GetType();
		if (ImGui::CollapsingHeader(type.name))
		{		
			for (auto property : type.properties)
				DisplayProperty(property);
		}

		ImGui::PopID();
	}

	ImGui::End();
}

void PropertyPanel::DisplayProperty(const Varlet::Property& property) const
{
	constexpr float speed = 0.33f;

	switch (property.type)
	{
	case Varlet::Type::Bool:
		ImGui::Checkbox(property.name, reinterpret_cast<bool*>(property.value));
		break;

	case Varlet::Type::Int8:
		ImGui::DragScalar(property.name, ImGuiDataType_S8, reinterpret_cast<int8_t*>(property.value), speed);
		break;

	case Varlet::Type::UInt8:
		ImGui::DragScalar(property.name, ImGuiDataType_U8, reinterpret_cast<uint8_t*>(property.value), speed);
		break;

	case Varlet::Type::Int16:
		ImGui::DragScalar(property.name, ImGuiDataType_S16, reinterpret_cast<int16_t*>(property.value), speed);
		break;

	case Varlet::Type::UInt16:
		ImGui::DragScalar(property.name, ImGuiDataType_U16, reinterpret_cast<uint16_t*>(property.value), speed);
		break;

	case Varlet::Type::Int32:
		ImGui::DragScalar(property.name, ImGuiDataType_S32, reinterpret_cast<int32_t*>(property.value), speed);
		break;

	case Varlet::Type::UInt32:
		ImGui::DragScalar(property.name, ImGuiDataType_U32, reinterpret_cast<uint32_t*>(property.value), speed);
		break;

	case Varlet::Type::Int64:
		ImGui::DragScalar(property.name, ImGuiDataType_S64, reinterpret_cast<int64_t*>(property.value), speed);
		break;

	case Varlet::Type::UInt64:
		ImGui::DragScalar(property.name, ImGuiDataType_U64, reinterpret_cast<uint64_t*>(property.value), speed);
		break;

	case Varlet::Type::Float:
		ImGui::DragScalar(property.name, ImGuiDataType_Float, reinterpret_cast<float*>(property.value), speed);
		break;

	case Varlet::Type::Double:
		ImGui::DragScalar(property.name, ImGuiDataType_Double, reinterpret_cast<double*>(property.value), speed);
		break;

	case Varlet::Type::Vector2:
		ImGui::DragFloat2(property.name, reinterpret_cast<float*>(property.value), speed);
		break;

	case Varlet::Type::Vector3:
		ImGui::DragFloat3(property.name, reinterpret_cast<float*>(property.value), speed);
		break;

	case Varlet::Type::Vector4:
		ImGui::DragFloat4(property.name, reinterpret_cast<float*>(property.value), speed);
		break;
	}
}
