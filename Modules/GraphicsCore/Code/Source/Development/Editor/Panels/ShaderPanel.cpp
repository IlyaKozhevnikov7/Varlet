#include "Development/Editor/Panels/ShaderPanel.h"
#include "ProjectAssetRegister.h"
#include "../../GraphicsInterface/Code/Include/GraphicsAPI.h"
#include "ShaderFormatter.h"
#include "Platform/Current.h"

#include <fstream>

void ShaderContextPanel::Update()
{
	auto shaderPtr = reinterpret_cast<Shader*>(_context);
	auto uniformInfos = Varlet::Graphics::GetShaderUniformInfos(shaderPtr);
	
	ImGuiAPI::Begin(_name.c_str(), &_isOpen);
	
	for (auto& info : (*uniformInfos))
		ImGuiAPI::Text("%s: %s", Varlet::Graphics::ShaderFormatter::ConvertShaderDataTypeToString(info.type).c_str(), info.name.c_str());
	
	ImGuiAPI::End();
}

void ShaderInitializationPanel::Update()
{
	ImGuiAPI::OpenPopup("Create New Shader");
	
	ImVec2 center = ImGuiAPI::GetMainViewport()->GetCenter();
	ImGuiAPI::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	
	if (ImGuiAPI::BeginPopupModal("Create New Shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char input[128];
		ImGuiAPI::InputText("Name", input, IM_ARRAYSIZE(input));
	
		if (ImGuiAPI::Button("OK", ImVec2(120, 0)) && input[0] != '\0')
		{
			const std::wstring validInput = Varlet::Core::Platform::ConvertToWide(input);
	
			const std::wstring assetName = _directory + WIDE("\\") + validInput + WIDE(".vasset");
			const std::wstring nativeShaderName = _directory + WIDE("\\") + validInput + WIDE(".vshader");
	
			CreateNativeShaderFile(nativeShaderName);
	
			ProjectAssetRegister::AddAsset(assetName, Shader::Create(nativeShaderName.c_str()));
	
			*_isSuccessful = true;
			_isOpen = false;
	
			ImGuiAPI::CloseCurrentPopup();
		}
	
		ImGuiAPI::SetItemDefaultFocus();
		ImGuiAPI::SameLine();
	
		if (ImGuiAPI::Button("Cancel", ImVec2(120, 0)))
		{
			*_isSuccessful = false;
			_isOpen = false;
			ImGuiAPI::CloseCurrentPopup();
		}
	
		ImGuiAPI::EndPopup();
	}
}

void ShaderInitializationPanel::CreateNativeShaderFile(const std::wstring& name)
{
	constexpr const char* defaultShaderCode = R"(.VERTEX_STAGE:
{
	[uniform_set(fast)]
	{
		mat4 u_View;
		mat4 u_Projection;
		mat4 u_Model;
	};

	void main()
	{
		gl_Position = u_Projection * u_View * u_Model * vec4(POSITION, 1.f);
	}
}
.FRAGMENT_STAGE:{

	[uniform_set] 
	{
		vec3 u_Color;
	};

	out vec4 fragColor;

	void main()
	{
		fragColor = vec4(u_Color, 1.f);
	}
}
)";

	std::ofstream file(name);

	file << defaultShaderCode;
	file.close();
}
