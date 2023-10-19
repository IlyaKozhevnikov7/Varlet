#pragma once

#include "EditorCore.h"
#include "EditorPanel.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/ImGuizmo.h"
#include "ImGuiPanel.generated.h"

/*
 * Base class for all imgui editor panels
 */
REFLECTABLE(Abstract)
class EDITOR_API ImGuiPanel : public EditorPanel
{
	GENERATION

public:

	virtual ~ImGuiPanel() = default;

	virtual void Initialize();

	virtual void Update() = 0;
};

REFLECTABLE(Interface)
class IClosablePanel
{
	GENERATION

protected:

	bool _isOpen = true;

public:

	virtual ~IClosablePanel() = default;

	bool ShouldClose() const;
};

REFLECTABLE(Abstract)
class EDITOR_API InitializationAssetPanelBase : public ImGuiPanel, public IClosablePanel
{
	GENERATION

protected:

	std::wstring _directory;
	bool* _isSuccessful;

public:

	virtual ~InitializationAssetPanelBase() = default;

	void Initialize(std::wstring& directory, bool& isSuccessful);

	virtual const Varlet::Core::Type* GetContextType() = 0;
};

REFLECTABLE(Abstract)
template<typename T>
class EDITOR_API InitializationAssetPanel : public InitializationAssetPanelBase
{
	GENERATION

public:

	virtual ~InitializationAssetPanel() = default;

	const Varlet::Core::Type* GetContextType()
	{
		return T::GetTypeStatic();
	}
};

REFLECTABLE(Abstract)
class EDITOR_API ImGuiContextPanelBase : public ImGuiPanel, public IClosablePanel
{
	GENERATION

protected:

	std::string _name;
	void* _context;

public:

	virtual ~ImGuiContextPanelBase() = default;

	void Initialize(const char* name, void* context);

	bool IsSameContext(void* context) const;

	virtual const Varlet::Core::Type* GetContextType() = 0;
};

REFLECTABLE(Abstract)
template<typename T>
class ImGuiContextPanel : public ImGuiContextPanelBase
{
	GENERATION

public:

	virtual ~ImGuiContextPanel() = default;

	const Varlet::Core::Type* GetContextType() override
	{
		return T::GetTypeStatic();
	}
};

/*
 * Base class for all editor panels witch display on property panel
 */
REFLECTABLE(Abstract)
class ImGuiPropertyPanel : public EditorPanel
{
	GENERATION

public:

	virtual ~ImGuiPropertyPanel() = default;

	virtual void Display(const char* name, void* context, bool isPointer) = 0;

	virtual const Varlet::Core::Type* GetContextType() = 0;
};

REFLECTABLE()
class DefaultImGuiPropertyPanel final : public ImGuiPropertyPanel
{
	GENERATION

public:

	const Varlet::Core::Type* contextType;

public:

	~DefaultImGuiPropertyPanel() override = default;

	void Display(const char* name, void* context, bool isPointer) override;

	const Varlet::Core::Type* GetContextType() override;

private:

	void DisplayPointer(const char* name, void* value);

	void DisplayInlineData(const char* name, void* value);
};

class EDITOR_API ImGuiAPI final
{
public:

	static void PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);

	static void PopStyleVar();

	static bool Begin(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);

	static void End();

	static ImVec2 GetContentRegionAvail();

	static ImVec2 GetWindowPos();

	static ImGuiViewport* GetMainViewport();

	static ImVec2 GetMousePos();

	static bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false);

	static bool IsMouseReleased(ImGuiMouseButton button);

	static void SetMouseCursor(ImGuiMouseCursor cursor_type);

	static bool IsKeyPressed(ImGuiKey key, bool repeat = true);

	static bool IsKeyDown(ImGuiKey key);

	static bool IsWindowHovered(ImGuiHoveredFlags flags = 0);

	static void Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));

	static void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);

	static void CloseCurrentPopup();

	static bool BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);

	static void EndPopup();

	static void SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0));

	static bool BeginPopupModal(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);

	static void Text(const char* fmt, ...);

	static void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);

	static bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

	static void SetItemDefaultFocus();

	static bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);

	static void EndCombo();

	static bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

	static bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));

	static void TreePop();

	static bool DragFloat(const char* label, float* v, float v_speed = 1.f, float v_min = 0.f, float v_max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	static void PushID(const void* ptr_id);

	static void PopID();

	static bool ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	static bool BeginDragDropTarget();

	static void EndDragDropTarget();

	static const ImGuiPayload* AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);



	static bool GizmosIsUsing();

	static bool GizmosIsOver();

	static void GizmosSetOrthographic(bool isOrthographic);

	static void GizmosSetDrawlist(ImDrawList* drawlist = nullptr);

	static void GizmosSetRect(float x, float y, float width, float height);

	static bool GizmosManipulate(const float* view, const float* projection, ImGuizmo::OPERATION operation, ImGuizmo::MODE mode, float* matrix, float* deltaMatrix = nullptr, const float* snap = nullptr, const float* localBounds = nullptr, const float* boundsSnap = nullptr);
};