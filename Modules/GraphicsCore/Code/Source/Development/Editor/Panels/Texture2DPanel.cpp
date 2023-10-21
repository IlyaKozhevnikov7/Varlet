#include "Development/Editor/Panels/Texture2DPanel.h"
#include "ProjectAssetRegister.h"
#include "InternalCall.h"
#include "Platform/Current.h"

void Texture2DPropertyPanel::Display(const char* name, void* value, bool isPointer)
{
	assert(isPointer);

	constexpr float buttonSize = 75.f;

	void** texturePtr = reinterpret_cast<void**>(value);

	const ImTextureID textureID = *texturePtr == nullptr
		? 0
		: NATIVE_TEXTURE_TO_IMTEXTUREID(Varlet::Graphics::InternalCall::GetNativeTexture(*texturePtr));

	ImGuiAPI::PushID(texturePtr);

	ImGuiAPI::ImageButton(textureID, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 }, 1);

	if (ImGuiAPI::BeginPopupContextItem("TextureContext"))
	{
		if (ImGuiAPI::Selectable("Clear"))
			*texturePtr = nullptr;

		ImGuiAPI::EndPopup();
	}

	if (ImGuiAPI::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGuiAPI::AcceptDragDropPayload(Texture2D::GetTypeStatic()->name.c_str()))
		{
			auto pathStr = std::bit_cast<const wchar_t*>(payload->Data);

			std::wstring path(pathStr, pathStr + payload->DataSize / sizeof(wchar_t));
			*texturePtr = Cast<Texture2D>(Varlet::AssetRegister::Load(path));
		}
		
		ImGuiAPI::EndDragDropTarget();
	}

	ImGuiAPI::PopID();

	ImGuiAPI::SameLine();
	ImGuiAPI::Text(name);
}

const Varlet::Core::Type* Texture2DPropertyPanel::GetContextType()
{
	return Texture2D::GetTypeStatic();
}

void Texture2DInitializationPanel::Update()
{
	const std::wstring nativeTexturePath = Varlet::Core::Platform::CreateFileDialog(WIDE("*.jpg;*.png"));
	
	const bool isSuccessful = nativeTexturePath.empty() == false;
	*_isSuccessful = isSuccessful;
	_isOpen = false;
	
	if (isSuccessful == false)
		return;
	
	if (const auto newTexture = Texture2D::Load(nativeTexturePath.c_str()))
	{
		const int32_t dotPosition = nativeTexturePath.find_first_of(WIDE('.'));
		std::wstring_view pathWithoutExtension();

		std::wstring assetPath(nativeTexturePath.c_str(), nativeTexturePath.c_str() + dotPosition);
		assetPath += WIDE(".vasset");

		ProjectAssetRegister::AddAsset(assetPath, newTexture);
	}
}
