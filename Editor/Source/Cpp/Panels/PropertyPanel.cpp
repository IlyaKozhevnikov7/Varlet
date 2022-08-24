#include "PropertyPanel.h"

#include "EditorCore.h"
#include "VarletFramework.h"

void PropertyPanel::Update()
{
	ImGui::Begin("Property");

	if (EditorData::selectedEntity != nullptr)
	{
		const auto components = EditorData::selectedEntity->GetComponents();
		for (auto component : components)
		{
			ImGui::PushID(component);
			DispalyObject(component);
			ImGui::PopID();
		};
	}

	ImGui::End();
}

void PropertyPanel::DisplayProperty(const Varlet::Property* property) const
{
	constexpr float speed = 0.33f;

	switch (property->type)
	{
	case Varlet::Type::Bool:
		ImGui::Checkbox(property->name, reinterpret_cast<bool*>(property->value));
		break;

	case Varlet::Type::Int8:
		ImGui::DragScalar(property->name, ImGuiDataType_S8, property->value, speed);
		break;

	case Varlet::Type::UInt8:
		ImGui::DragScalar(property->name, ImGuiDataType_U8, property->value, speed);
		break;

	case Varlet::Type::Int16:
		ImGui::DragScalar(property->name, ImGuiDataType_S16, property->value, speed);
		break;

	case Varlet::Type::UInt16:
		ImGui::DragScalar(property->name, ImGuiDataType_U16, property->value, speed);
		break;

	case Varlet::Type::Int32:
		ImGui::DragScalar(property->name, ImGuiDataType_S32, property->value, speed);
		break;

	case Varlet::Type::UInt32:
		ImGui::DragScalar(property->name, ImGuiDataType_U32, property->value, speed);
		break;

	case Varlet::Type::Int64:
		ImGui::DragScalar(property->name, ImGuiDataType_S64, property->value, speed);
		break;

	case Varlet::Type::UInt64:
		ImGui::DragScalar(property->name, ImGuiDataType_U64, property->value, speed);
		break;

	case Varlet::Type::Float:
		ImGui::DragScalar(property->name, ImGuiDataType_Float, property->value, speed);
		break;

	case Varlet::Type::Double:
		ImGui::DragScalar(property->name, ImGuiDataType_Double, property->value, speed);
		break;

	case Varlet::Type::Vector2:
		ImGui::DragScalarN(property->name, ImGuiDataType_Float, property->value, 2, speed);
		break;

	case Varlet::Type::Vector3:
		ImGui::DragScalarN(property->name, ImGuiDataType_Float, property->value, 3, speed);
		break;

	case Varlet::Type::Vector4:
		ImGui::DragScalarN(property->name, ImGuiDataType_Float, property->value, 4, speed);
		break;

		// TODO
	case Varlet::Type::BoolVector2:
	case Varlet::Type::BoolVector3:
	case Varlet::Type::BoolVector4:
		break;

	case Varlet::Type::Int32Vector2:
		ImGui::DragScalarN(property->name, ImGuiDataType_S32, property->value, 2, speed);
		break;

	case Varlet::Type::Int32Vector3:
		ImGui::DragScalarN(property->name, ImGuiDataType_S32, property->value, 3, speed);
		break;

	case Varlet::Type::Int32Vector4:
		ImGui::DragScalarN(property->name, ImGuiDataType_S32, property->value, 4, speed);
		break;

	case Varlet::Type::UInt32Vector2:
		ImGui::DragScalarN(property->name, ImGuiDataType_U32, property->value, 2, speed);
		break;

	case Varlet::Type::UInt32Vector3:
		ImGui::DragScalarN(property->name, ImGuiDataType_U32, property->value, 3, speed);
		break;

	case Varlet::Type::UInt32Vector4:
		ImGui::DragScalarN(property->name, ImGuiDataType_U32, property->value, 4, speed);
		break;

	case Varlet::Type::DoubleVector2:
		ImGui::DragScalarN(property->name, ImGuiDataType_Double, property->value, 2, speed);
		break;

	case Varlet::Type::DoubleVector3:
		ImGui::DragScalarN(property->name, ImGuiDataType_Double, property->value, 3, speed);
		break;

	case Varlet::Type::DoubleVector4:
		ImGui::DragScalarN(property->name, ImGuiDataType_Double, property->value, 4, speed);
		break;

	case Varlet::Type::Object:
		DispalyObject(static_cast<Object*>(property->value));
		break;

	case Varlet::Type::Array:
		DispalyArray(static_cast<const Varlet::Array*>(property));
		break;

	case Varlet::Type::Color3:
		ImGui::ColorEdit3(property->name, reinterpret_cast<float*>(property->value), ImGuiColorEditFlags_Float);
		break;

	case Varlet::Type::Color4:
		ImGui::ColorEdit4(property->name, reinterpret_cast<float*>(property->value), ImGuiColorEditFlags_Float);
		break;

	case Varlet::Type::Sampler2D:
	{
		constexpr float buttonSize = 75.f;

		auto texturePtr = static_cast<Varlet::Texture**>(property->value);
		const uint32_t previewTextureId = *texturePtr == nullptr ? 0 : static_cast<Varlet::Texture*>(*texturePtr)->GetId();

		ImGui::ImageButton(reinterpret_cast<ImTextureID>(previewTextureId), { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 }, 1);

		if (ImGui::BeginPopupContextItem("TextureContext"))
		{
			if (ImGui::Selectable("Clear"))
				*texturePtr = nullptr;

			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(TEXTURE_FILE_PAYLOAD))
			{
				auto path = static_cast<const char*>(payload->Data);

				LoadableTextureConfiguration configuration;
				configuration.path = path;
				auto newTexture = Varlet::RendererAPI::LoadTexture(configuration);

				*texturePtr = newTexture.get();
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		ImGui::Text(property->name);
	}
		break;
	case Varlet::Type::SamplerCube:

		break;

	default:
		break;
	}
}

void PropertyPanel::DispalyObject(Object* object, const int32_t& id) const
{
	auto type = object->GetType();
	std::string name(type.name);

	if (id > -1)
		name.append(": " + std::to_string(id));

	if (ImGui::TreeNode(name.c_str()))
	{
		for (auto property : type.properties)
			DisplayProperty(property);

		ImGui::TreePop();
	}
}

void PropertyPanel::DispalyArray(const Varlet::Array* property) const
{
#define DISPLAY_ELEMENTS(T) \
	{ \
		std::vector<T>* elements = reinterpret_cast<std::vector<T>*>(property->value); \
		for (auto& element : *elements) \
		{ \
			ImGui::PushID(&element); \
			const std::string name("Element: " + std::to_string(elementId)); \
			Varlet::Property property(name.c_str(), element); \
			DisplayProperty(&property); \
			ImGui::PopID(); \
			++elementId; \
		} \
	} \

	if (ImGui::TreeNode(property->name))
	{
		int32_t elementId = 0;

		switch (property->contentType)
		{
		case Varlet::Type::Object:
		{
			std::vector<Object*>* elements = reinterpret_cast<std::vector<Object*>*>(property->value);

			for (auto element : *elements)
			{
				ImGui::PushID(element);
				DispalyObject(element, elementId);
				ImGui::PopID();
				++elementId;
			}
		}
		break;

		case Varlet::Type::Bool:
			break;

		case Varlet::Type::Int8:			DISPLAY_ELEMENTS(int8_t);		break;
		case Varlet::Type::UInt8:			DISPLAY_ELEMENTS(uint8_t);		break;
		case Varlet::Type::Int16:			DISPLAY_ELEMENTS(int16_t);		break;
		case Varlet::Type::UInt16:			DISPLAY_ELEMENTS(uint16_t);		break;
		case Varlet::Type::Int32:			DISPLAY_ELEMENTS(int32_t);		break;
		case Varlet::Type::UInt32:			DISPLAY_ELEMENTS(uint32_t);		break;
		case Varlet::Type::Int64:			DISPLAY_ELEMENTS(int64_t);		break;
		case Varlet::Type::UInt64:			DISPLAY_ELEMENTS(uint64_t);		break;
		case Varlet::Type::Float:			DISPLAY_ELEMENTS(float);		break;
		case Varlet::Type::Double:			DISPLAY_ELEMENTS(double);		break;
		case Varlet::Type::Vector2:			DISPLAY_ELEMENTS(glm::vec2);	break;
		case Varlet::Type::Vector3:			DISPLAY_ELEMENTS(glm::vec3);	break;
		case Varlet::Type::Vector4:			DISPLAY_ELEMENTS(glm::vec4);	break;
		case Varlet::Type::BoolVector2:		DISPLAY_ELEMENTS(glm::bvec2);	break;
		case Varlet::Type::BoolVector3:		DISPLAY_ELEMENTS(glm::bvec3);	break;
		case Varlet::Type::BoolVector4:		DISPLAY_ELEMENTS(glm::bvec4);	break;
		case Varlet::Type::Int32Vector2:	DISPLAY_ELEMENTS(glm::ivec2);	break;
		case Varlet::Type::Int32Vector3:	DISPLAY_ELEMENTS(glm::ivec3);	break;
		case Varlet::Type::Int32Vector4:	DISPLAY_ELEMENTS(glm::ivec4);	break;
		case Varlet::Type::UInt32Vector2:	DISPLAY_ELEMENTS(glm::uvec2);	break;
		case Varlet::Type::UInt32Vector3:	DISPLAY_ELEMENTS(glm::uvec3);	break;
		case Varlet::Type::UInt32Vector4:	DISPLAY_ELEMENTS(glm::uvec4);	break;
		case Varlet::Type::DoubleVector2:	DISPLAY_ELEMENTS(glm::dvec2);	break;
		case Varlet::Type::DoubleVector3:	DISPLAY_ELEMENTS(glm::dvec3);	break;
		case Varlet::Type::DoubleVector4:	DISPLAY_ELEMENTS(glm::dvec4);	break;
		
		case Varlet::Type::Matrix2:
		case Varlet::Type::Matrix3:
		case Varlet::Type::Matrix4:

		case Varlet::Type::Color3:
		case Varlet::Type::Color4:

		case Varlet::Type::Sampler2D:
		case Varlet::Type::SamplerCube:
			break;

		default:
			break;
		}

		ImGui::TreePop();
	}
}
