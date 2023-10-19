#include "ShaderFormatter.h"
#include "Platform/Current.h"

#include <regex>

namespace
{
	struct BlockDefinition final
	{
		std::string name;
		int32_t position;
	};
}

namespace Varlet::Graphics
{
	ShaderFormatter::ShaderFormatter(const std::string& source)
	{
		const std::regex regex("\\.(\\w+):");
		std::smatch matches;
		std::string suffix = source;

		std::vector<BlockDefinition> blockDefinitions;
		int32_t additionalOffset = 0;

		while (std::regex_search(suffix, matches, regex))
		{
			BlockDefinition definition;
			definition.name = matches[1].str();
			definition.position = matches.position() + additionalOffset;
			additionalOffset = definition.position + matches.length();

			blockDefinitions.push_back(definition);
			suffix = matches.suffix();
		}

		const int32_t definitionsCount = blockDefinitions.size();
		bool hasStateDefinition = false;

		for (int32_t i = 0; i < definitionsCount; i++)
		{
			const std::string blockName = blockDefinitions[i].name;
			ShaderStage currentShaderStage;
			bool isStateDefinition = false;

			if (blockName == "VERTEX_STAGE")
			{
				currentShaderStage = ShaderStage::Vertex;
			}
			else if (blockName == "FRAGMENT_STAGE")
			{
				currentShaderStage = ShaderStage::Fragment;
			}
			else if (blockName == "GEOMETRY_STAGE")
			{
				currentShaderStage = ShaderStage::Geometry;
			}
			else if (hasStateDefinition == false && blockName == "STATE")
			{
				isStateDefinition = true;
			}
			else
			{
				const std::wstring wideStageName = Varlet::Core::Platform::ConvertToWide(blockDefinitions[i].name.c_str());
				VARLET_LOG(Warning, WIDE("Shader has invalid stage: %s"), wideStageName.c_str());
				continue;
			}

			const char* partStart = source.c_str() + blockDefinitions[i].position;

			const int32_t partEnd = i == definitionsCount - 1
				? source.size()
				: blockDefinitions[i + 1].position;

			const std::string_view part(partStart, partEnd - blockDefinitions[i].position);

			const int32_t startCode = part.find_first_of('{') + 1;
			const int32_t endCode = part.find_last_of('}') - 1;

			std::string blockSource(part.substr(startCode, endCode - startCode));

			if (isStateDefinition)
			{
				ProcessStates(blockSource);
			}
			else
			{
				_stages[currentShaderStage] = blockSource;
			}
		}
	}

	const std::unordered_map<ShaderStage, std::string>& ShaderFormatter::GetStages() const
	{
		return _stages;
	}

	std::string ShaderFormatter::GetSource() const
	{
		std::string source;

		for (auto& stage : _stages)
		{
			std::string shaderStage;

			switch (stage.first)
			{
			case ShaderStage::Vertex:	shaderStage = ".VERTEX_STAGE:{";	break;
			case ShaderStage::Geometry:	shaderStage = ".GEOMETRY_STAGE:{";	break;
			case ShaderStage::Fragment:	shaderStage = ".FRAGMENT_STAGE:{";	break;
			}

			assert(shaderStage.empty() == false);
			source += shaderStage + stage.second + "\n}\n";
		}

		return source;
	}

	bool ShaderFormatter::HasStage(ShaderStage stage) const
	{
		return _stages.contains(stage);
	}

	void ShaderFormatter::OverwriteStage(ShaderStage stage, const std::string& source)
	{
		_stages[stage] = source;
	}

	void ShaderFormatter::AddVertextAttribute(const char* name, ShaderDataType type)
	{
		assert(_stages.contains(ShaderStage::Vertex));

		auto& vertexStage = _stages[ShaderStage::Vertex];

		vertexStage = "[assembly(" + std::to_string(_assemblyCount++) + ")] " + ConvertShaderDataTypeToString(type) + " " + name + ";\n" + vertexStage;
	}

	void ShaderFormatter::AddCode(const std::string& code, ShaderStage stage)
	{
		assert(_stages.contains(stage));

		auto& stageCode = _stages[stage];
		stageCode = code + "\n" + stageCode;
	}

	void ShaderFormatter::ReplaceInputAssemly(ReplaceInputAssemblySignature replaceFunction)
	{
		// TODO: process only for vertex shader

		assert(replaceFunction);

		const std::regex regex("\\[assembly\\((\\d+)\\)\\]\\s+(\\S+)\\s+(\\S+)\\s*;");
		std::smatch matches;

		for (auto& stage : _stages)
		{
			std::string suffix = stage.second;
			ShaderInputAssemblyInfo info;

			size_t lastPos = 0;

			while (std::regex_search(suffix, matches, regex))
			{
				info.name = matches[3];
				info.location = std::stoi(matches[1]);
				info.typeName = matches[2];

				const std::string toReplace = replaceFunction(info);
				lastPos += matches.prefix().str().size();
				auto length = matches[0].str().size();
				
				stage.second.replace(lastPos, length, toReplace);
				lastPos += toReplace.size();
				
				suffix = matches.suffix();
			}
		}
	}

	void ShaderFormatter::ReplaceUniformSets(ReplaceUniformSetsSignature replaceFunction)
	{
		assert(replaceFunction);

		const std::regex setPattern("\\[uniform_set(\\((.+)\\))?\\]\\s+(\\S+)?\\s+\\{([\\s\\S]*?)\\};");
		std::smatch setMatches;

		for (auto& stage : _stages)
		{
			std::string setSuffix = stage.second;

			size_t lastPos = 0;

			while (std::regex_search(setSuffix, setMatches, setPattern))
			{
				ShaderUniformSetInfo info;

				info.name = setMatches[3];
				info.meta = setMatches[2];

				const std::regex valuePattern("\\s*(\\S+)\\s+(\\S+).*?;");
				std::smatch valueMatches;
				std::string valueSuffix = setMatches[4];

				while (std::regex_search(valueSuffix, valueMatches, valuePattern))
				{
					info.values.push_back(
						{
							valueMatches[2],
							valueMatches[1]
						});

					valueSuffix = valueMatches.suffix();
				}
				
				const std::string toReplace = replaceFunction(info);
				lastPos += setMatches.prefix().str().size();
				auto length = setMatches[0].str().size();

				stage.second.replace(lastPos, length, toReplace);
				lastPos += toReplace.size();

				setSuffix = setMatches.suffix();
			}
		}
	}

	bool ShaderFormatter::HasStateDefinition() const
	{
		return _states.empty() == false;
	}

	const std::vector<std::string>* ShaderFormatter::GetStateValues(const std::string& stateName)
	{
		return _states.contains(stateName)
			? &_states[stateName]
			: nullptr;
	}

	std::string ShaderFormatter::ConvertShaderDataTypeToString(ShaderDataType type)
	{
		switch (type)
		{
		case Varlet::Graphics::ShaderDataType::Bool:		return "float";
		case Varlet::Graphics::ShaderDataType::Int:			return "int";
		case Varlet::Graphics::ShaderDataType::UInt:		return "uint";
		case Varlet::Graphics::ShaderDataType::Float:		return "float";
		case Varlet::Graphics::ShaderDataType::Double:		return "double";
		case Varlet::Graphics::ShaderDataType::Vec2:		return "vec2";
		case Varlet::Graphics::ShaderDataType::Vec3:		return "vec3";
		case Varlet::Graphics::ShaderDataType::Vec4:		return "vec4";
		case Varlet::Graphics::ShaderDataType::Mat2:		return "mat2";
		case Varlet::Graphics::ShaderDataType::Mat3:		return "mat3";
		case Varlet::Graphics::ShaderDataType::Mat4:		return "mat4";
		case Varlet::Graphics::ShaderDataType::Sampler2D:	return "sampler2D";
		}

		VARLET_ERROR("Unknown shader data type");
		return "";
	}

	void ShaderFormatter::ProcessStates(const std::string& state)
	{
		const std::regex statePattern("(\\S+):\\s*(.+)");
		std::smatch stateMatches;
		std::string stateSuffix = state;

		const std::regex paramsPattern("(\\S+)");
		std::smatch paramsMatches;

		while (std::regex_search(stateSuffix, stateMatches, statePattern))
		{
			const std::string stateName = stateMatches[1];
			std::string stateParams = stateMatches[2];

			while (std::regex_search(stateParams, paramsMatches, paramsPattern))
			{
				_states[stateName].push_back(paramsMatches[1]);
				stateParams = paramsMatches.suffix();
			}

			stateSuffix = stateMatches.suffix();
		}
	}

	BitMask<ShaderUniformSetFlags> ShaderFormatter::ConvertToShaderUniformSetFlags(std::string string)
	{
		BitMask<ShaderUniformSetFlags> flags;
		
		if (string.find("fast") != std::string::npos)
			flags |= ShaderUniformSetFlags::Fast;

		if (string.find("shared") != std::string::npos)
			flags |= ShaderUniformSetFlags::Shared;

		return flags;
	}
}
