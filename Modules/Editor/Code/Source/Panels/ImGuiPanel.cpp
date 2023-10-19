#include "Panels/ImGuiPanel.h"
#include "Asset.h"
#include "UIService.h"
#include "EditorUtilitySystem.h"
#include "Utilities/ImGuiPanelUtility.h"

void ImGuiPanel::Initialize()
{
}

bool IClosablePanel::ShouldClose() const
{
	return _isOpen == false;
}

void InitializationAssetPanelBase::Initialize(std::wstring& directory, bool& isSuccessful)
{
	_directory = directory;
	_isSuccessful = &isSuccessful;

	BaseClass::Initialize();
}

void ImGuiContextPanelBase::Initialize(const char* name, void* context)
{
	_name = name;
	_context = context;

	BaseClass::Initialize();
}

bool ImGuiContextPanelBase::IsSameContext(void* context) const
{
	return _context == context;
}

void DefaultImGuiPropertyPanel::Display(const char* name, void* context, bool isPointer)
{
	if (isPointer)
	{
		DisplayPointer(name, context);
	}
	else
	{
		DisplayInlineData(name, context);
	}
}

const Varlet::Core::Type* DefaultImGuiPropertyPanel::GetContextType()
{
	return nullptr;
}

void DefaultImGuiPropertyPanel::DisplayPointer(const char* name, void* value)
{
	void** actualValue = std::bit_cast<void**>(value);

	if (*actualValue == nullptr)
	{
		static char nullText[] = "NULL";
		ImGui::InputText(name, nullText, IM_ARRAYSIZE(nullText), ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		ImGui::InputText(name, (char*)*actualValue, sizeof(void*), ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginPopupContextItem(name))
		{
			if (ImGui::Selectable("Clear"))
				*actualValue = nullptr;
		
			ImGui::EndPopup();
		}

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			static ImGuiPanelUtility* panelUtility = EditorUtilitySystem::FindByType<ImGuiPanelUtility>();
			assert(panelUtility);

			if (auto panelType = panelUtility->GetContextPanelFor(contextType))
				panelUtility->OpenContextPanel(panelType, contextType->name.c_str(), *actualValue);
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(contextType->name.c_str()))
		{
			if (contextType->IsA(IAsset))
			{
				auto pathStr = std::bit_cast<const wchar_t*>(payload->Data);

				std::wstring path(pathStr, pathStr + payload->DataSize / sizeof(wchar_t));
				*actualValue = Varlet::AssetRegister::Load(path);
			}
			else
			{
				*actualValue = *std::bit_cast<void**>(payload->Data);
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void DefaultImGuiPropertyPanel::DisplayInlineData(const char* name, void* value)
{
	if (ImGui::TreeNodeDefaultOpen(name))
	{
		for (const auto& field : contextType->fieldInfos)
		{
			if (field.flags.NoHas(Varlet::Core::EditorAccessible))
				continue;

			int8_t* location = field.flags.Has(Varlet::Core::Static)
				? 0
				: std::bit_cast<int8_t*>(value);

			int8_t* value = location + field.offset;

			UIService::DisplayValue(field.name.c_str(), field.typeId, value);
		}

		ImGui::TreePop();
	}
}

void ImGuiAPI::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
{
	ImGui::PushStyleVar(idx, val);
}

void ImGuiAPI::PopStyleVar()
{
	ImGui::PopStyleVar();
}

bool ImGuiAPI::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
	return ImGui::Begin(name, p_open, flags);
}

void ImGuiAPI::End()
{
	ImGui::End();
}

ImVec2 ImGuiAPI::GetContentRegionAvail()
{
	return ImGui::GetContentRegionAvail();
}

ImVec2 ImGuiAPI::GetWindowPos()
{
	return ImGui::GetWindowPos();
}

ImGuiViewport* ImGuiAPI::GetMainViewport()
{
	return ImGui::GetMainViewport();
}

ImVec2 ImGuiAPI::GetMousePos()
{
	return ImGui::GetMousePos();
}

bool ImGuiAPI::IsMouseClicked(ImGuiMouseButton button, bool repeat)
{
	return ImGui::IsMouseClicked(button, repeat);
}

bool ImGuiAPI::IsMouseReleased(ImGuiMouseButton button)
{
	return ImGui::IsMouseReleased(button);
}

void ImGuiAPI::SetMouseCursor(ImGuiMouseCursor cursor_type)
{
	ImGui::SetMouseCursor(cursor_type);
}

bool ImGuiAPI::IsKeyPressed(ImGuiKey key, bool repeat)
{
	return ImGui::IsKeyPressed(key, repeat);
}

bool ImGuiAPI::IsKeyDown(ImGuiKey key)
{
	return ImGui::IsKeyDown(key);
}

bool ImGuiAPI::IsWindowHovered(ImGuiHoveredFlags flags)
{
	return ImGui::IsWindowHovered(flags);
}

void ImGuiAPI::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
}

void ImGuiAPI::OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags)
{
	ImGui::OpenPopup(str_id, popup_flags);
}

void ImGuiAPI::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

bool ImGuiAPI::BeginPopupContextItem(const char* str_id, ImGuiPopupFlags popup_flags)
{
	return ImGui::BeginPopupContextItem(str_id, popup_flags);
}

void ImGuiAPI::EndPopup()
{
	ImGui::EndPopup();
}

void ImGuiAPI::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
	ImGui::SetNextWindowPos(pos, cond, pivot);
}

bool ImGuiAPI::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
	return ImGui::BeginPopupModal(name, p_open, flags);
}

void ImGuiAPI::Text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	ImGui::Text(fmt, args);
	va_end(args);
}

void ImGuiAPI::SameLine(float offset_from_start_x, float spacing)
{
	ImGui::SameLine(offset_from_start_x, spacing);
}

bool ImGuiAPI::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	return ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
}

void ImGuiAPI::SetItemDefaultFocus()
{
	ImGui::SetItemDefaultFocus();
}

bool ImGuiAPI::BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
	return ImGui::BeginCombo(label, preview_value, flags);
}

void ImGuiAPI::EndCombo()
{
	ImGui::EndCombo();
}

bool ImGuiAPI::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size)
{
	return ImGui::Selectable(label, selected, flags, size);
}

bool ImGuiAPI::Button(const char* label, const ImVec2& size)
{
	return ImGui::Button(label, size);
}

void ImGuiAPI::TreePop()
{
	ImGui::TreePop();
}

bool ImGuiAPI::DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
}

void ImGuiAPI::PushID(const void* ptr_id)
{
	ImGui::PushID(ptr_id);
}

void ImGuiAPI::PopID()
{
	ImGui::PopID();
}

bool ImGuiAPI::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	return ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding, bg_col, tint_col);
}

bool ImGuiAPI::BeginDragDropTarget()
{
	return ImGui::BeginDragDropTarget();
}

void ImGuiAPI::EndDragDropTarget()
{
	ImGui::EndDragDropTarget();
}

const ImGuiPayload* ImGuiAPI::AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags)
{
	return ImGui::AcceptDragDropPayload(type, flags);
}

bool ImGuiAPI::GizmosIsUsing()
{
	return ImGuizmo::IsUsing();
}

bool ImGuiAPI::GizmosIsOver()
{
	return ImGuizmo::IsOver();
}

void ImGuiAPI::GizmosSetOrthographic(bool isOrthographic)
{
	ImGuizmo::SetOrthographic(isOrthographic);
}

void ImGuiAPI::GizmosSetDrawlist(ImDrawList* drawlist)
{
	ImGuizmo::SetDrawlist(drawlist);
}

void ImGuiAPI::GizmosSetRect(float x, float y, float width, float height)
{
	ImGuizmo::SetRect(x, y, width, height);
}

bool ImGuiAPI::GizmosManipulate(const float* view, const float* projection, ImGuizmo::OPERATION operation, ImGuizmo::MODE mode, float* matrix, float* deltaMatrix, const float* snap, const float* localBounds, const float* boundsSnap)
{
	return ImGuizmo::Manipulate(view, projection, operation, mode, matrix, deltaMatrix, snap, localBounds, boundsSnap);
}