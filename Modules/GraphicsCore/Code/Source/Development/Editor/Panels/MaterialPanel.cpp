#include "Development/Editor/Panels/MaterialPanel.h"
#include "Assets/Material.h"
#include "Assets/Texture.h"
#include "UIService.h"
#include "ProjectAssetRegister.h"

#include <locale>
#include <codecvt>

void MaterialContextPanel::Update()
{
	const static size_t uniformBufferOffset = 24;
	
	auto materialPtr = reinterpret_cast<Material*>(_context);
	std::vector<int8_t>* uniformBuffer = reinterpret_cast<std::vector<int8_t>*>(reinterpret_cast<int8_t*>(materialPtr) + uniformBufferOffset);
	
	auto shader = materialPtr->GetShader();
	if (shader == nullptr)
	{
		_isOpen = false;
		return;
	}
	
	auto& layoutInfo = materialPtr->GetUniformLayoutInfo();
	
	ImGuiAPI::Begin(_name.c_str(), &_isOpen);
	
	for (auto& info : layoutInfo.uniformInfos)
	{
		BitMask<Varlet::Core::FieldMetaFlags> marks = 0;
		const Varlet::Core::Type* type = info.second.type;

		if (type->IsA(Texture))
			marks |= Varlet::Core::FieldMetaFlags::Pointer;
	
		int8_t* value = (int8_t*)(&(*uniformBuffer)[0]) + info.second.offset;
		UIService::DisplayValue(info.first.c_str(), type->id, value, marks);
	}
	
	ImGuiAPI::End();
}

void MaterialInitializationPanel::Initialize()
{
	memset(_materialName, 0, MaxNameLength);

	_shaderPaths = ProjectAssetRegister::GetAllAvailableAssetsOf<Shader>();
	_shaderNames.resize(_shaderPaths.size());

	for (int32_t i = 0; i < _shaderPaths.size(); i++)
	{
		auto path = _shaderPaths[i];

		const auto namePosition = path.find_last_of(WIDE('\\')) + 1;
		const auto dotPosition = path.find_first_of(WIDE('.'));

		auto wName = path.substr(namePosition, dotPosition - namePosition);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		_shaderNames[i] = converter.to_bytes(&wName.front(), &wName.back() + 1);
	}
}

void MaterialInitializationPanel::Update()
{
	ImGuiAPI::OpenPopup("Create New Material");

	ImVec2 center = ImGuiAPI::GetMainViewport()->GetCenter();
	ImGuiAPI::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (_shaderNames.empty() == false)
	{
		DrawCreateMaterialMenu();
	}
	else
	{
		DrawNoShaderMenu();
	}
}

void MaterialInitializationPanel::DrawCreateMaterialMenu()
{
	if (ImGuiAPI::BeginPopupModal("Create New Material", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGuiAPI::Text("Name");
		ImGuiAPI::SameLine();

		ImGuiAPI::InputText("##materialName", _materialName, MaxNameLength);
		ImGuiAPI::SetItemDefaultFocus();

		ImGuiAPI::Text("Shader based");
		ImGuiAPI::SameLine();

		if (ImGuiAPI::BeginCombo("##shaderBased", _shaderNames[_selectedShaderIndex].c_str()))
		{
			for (int32_t i = 0; i < _shaderNames.size(); i++)
			{
				const bool isSelected = (_selectedShaderIndex == i);
				if (ImGuiAPI::Selectable(_shaderNames[i].c_str(), isSelected))
					_selectedShaderIndex = i;

				if (isSelected)
					ImGuiAPI::SetItemDefaultFocus();
			}
			ImGuiAPI::EndCombo();
		}

		if ((ImGuiAPI::Button("OK", ImVec2(120, 0)) || ImGuiAPI::IsKeyPressed(ImGuiKey_Enter)) && _materialName[0] != '\0')
		{
			Shader* shader = Cast<Shader>(Varlet::AssetRegister::Load({ _shaderPaths[_selectedShaderIndex].begin(), _shaderPaths[_selectedShaderIndex].end() }));

			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			const std::wstring validInput = converter.from_bytes(_materialName);
			const std::wstring assetName = _directory + WIDE("\\") + validInput + WIDE(".vasset");

			ProjectAssetRegister::AddAsset(assetName, new Material(shader));

			*_isSuccessful = true;
			_isOpen = false;

			ImGuiAPI::CloseCurrentPopup();
		}

		ImGuiAPI::SetItemDefaultFocus();
		ImGuiAPI::SameLine();

		if (ImGuiAPI::Button("Cancel", ImVec2(120, 0)))
		{
			*_isSuccessful = true;
			_isOpen = false;
			ImGuiAPI::CloseCurrentPopup();
		}

		ImGuiAPI::EndPopup();
	}
}

void MaterialInitializationPanel::DrawNoShaderMenu()
{
	if (ImGuiAPI::BeginPopupModal("Create New Material", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGuiAPI::Text("No shaders");

		if (ImGuiAPI::Button("Ok", ImVec2(120, 0)))
		{
			*_isSuccessful = false;
			_isOpen = false;
			ImGuiAPI::CloseCurrentPopup();
		}

		ImGuiAPI::EndPopup();
	}
}
