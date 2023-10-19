#pragma once

#include "GraphicsInterfaceCore.h"
#include "GraphicsBaseTypes.h"

namespace Varlet::Graphics
{
	/*
	*	Shader Format 
	* 
	*	--------------------------------------------------
	* 
	*	Available stages
	*		- .VERTEX_STAGE 
	*		- .FRAGMENT_STAGE
	* 
	*	--------------------------------------------------
	* 
	*	Stage format:
	*	
	*	.NAME_STAGE
	*	{
	*		// shader code here
	*	} 
	*/

	enum class ShaderStage : uint8_t
	{
		Vertex = 0,
		Geometry,
		Fragment,
	};

	enum ShaderUniformSetFlags : uint8_t
	{
		Fast = BIT<1>,
		Shared = BIT<2>
	};

	struct ShaderInputAssemblyInfo final
	{
		std::string name;
		uint32_t location;
		std::string typeName;
	};

	struct ShaderUniformSetInfo final
	{
		struct Value final
		{
			std::string name;
			std::string typeName;
		};

		std::string name;
		std::string meta;

		std::vector<Value> values;
	};

	struct ShaderUniformVariableInfo final
	{
		std::string name;
		std::string typeName;
		std::string meta;
	};

	struct ShaderInputAttachmentInfo final
	{
		std::string name;
		uint32_t location;
		std::string typeName;
	};

	class GRAPHICSINTERFACE_API ShaderFormatter final
	{
		using ReplaceInputAssemblySignature = std::string(*)(const ShaderInputAssemblyInfo&);
		using ReplaceUniformSetsSignature = std::string(*)(const ShaderUniformSetInfo&);
		using ReplaceUniformVariablesInfo = std::string(*)(const std::vector<ShaderUniformVariableInfo>&);

	private:

		uint32_t _assemblyCount = 0;

		std::unordered_map<std::string, std::vector<std::string>> _states;
		std::unordered_map<ShaderStage, std::string> _stages;

	public:

		ShaderFormatter(const std::string& source);

		const std::unordered_map<ShaderStage, std::string>& GetStages() const;

		std::string GetSource() const;

		bool HasStage(ShaderStage stage) const;

		void OverwriteStage(ShaderStage stage, const std::string& source);

		void AddVertextAttribute(const char* name, ShaderDataType type);

		void AddCode(const std::string& code, ShaderStage stage);

		void ReplaceInputAssemly(ReplaceInputAssemblySignature replaceFunction);

		void ReplaceUniformSets(ReplaceUniformSetsSignature replaceFunction);

		bool HasStateDefinition() const;

		const std::vector<std::string>* GetStateValues(const std::string& stateName);

		static std::string ConvertShaderDataTypeToString(ShaderDataType type);

	private:

		void ProcessStates(const std::string& state);

		BitMask<ShaderUniformSetFlags> ConvertToShaderUniformSetFlags(std::string string);
	};
}