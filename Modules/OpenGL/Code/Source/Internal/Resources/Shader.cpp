#include "Internal/Resources/Shader.h"
#include "Internal/Resources/UniformBuffer.h"
#include "ShaderFormatter.h"

#include <regex>

namespace Varlet::Graphics::OpenGL
{
	Shader::Shader(Shader&& other) noexcept
	{
		id = other.id;
		other.id = 0;

		blendState = other.blendState;
		depthWrite = other.depthWrite;
		stencilWrite = other.stencilWrite;
		cullFace = other.cullFace;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		id = other.id;
		other.id = 0;

		blendState = other.blendState;
		depthWrite = other.depthWrite;
		stencilWrite = other.stencilWrite;
		cullFace = other.cullFace;

		return *this;
	}

	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	ShaderCreateResult Shader::Create(const std::string& source)
	{
		std::vector<UniformInfo> uniformInfos;
		std::vector<GLint> uniformLocations;
		GLint firstTextureUnit = -1;

		Shader shader = Shader(source, uniformInfos, uniformLocations, firstTextureUnit);
		
		return {
			shader.id > 0,
			std::move(shader),
			std::move(uniformInfos),
			std::move(uniformLocations),
			(GLenum)firstTextureUnit
		};
	}

	Shader::Shader(const std::string& source, std::vector<UniformInfo>& uniformInfos, std::vector<GLint>& uniformLocations, GLint& firstTextureUnit)
	{
		std::vector<std::pair<ShaderStage, std::string>> stages = ConvertToGLSources(source);

		std::vector<GLuint> shaders;
		shaders.reserve(stages.size());

		bool isSuccess = true;
		id = glCreateProgram();

		for (auto& stage : stages)
		{
			GLenum shaderType = 0;

			switch (stage.first)
			{
			case ShaderStage::Vertex:	shaderType = GL_VERTEX_SHADER;		break;
			case ShaderStage::Geometry:	shaderType = GL_GEOMETRY_SHADER;	break;
			case ShaderStage::Fragment:	shaderType = GL_FRAGMENT_SHADER;	break;

			default: break;
			}

			const GLuint shaderId = GenerateShader(shaderType, stage.second.c_str());
			
			if (shaderId == 0)
			{
				isSuccess = false;
				break;
			}

			shaders.push_back(shaderId);
			glAttachShader(id, shaderId);
		}

		if (isSuccess)
		{
			glLinkProgram(id);
			isSuccess = Validate(id, ObjectType::Program);
		}

		for (auto shaderId : shaders)
		{
			glDetachShader(id, shaderId);
			glDeleteShader(shaderId);
		}

		if (isSuccess == false)
		{
			glDeleteProgram(id);
			id = 0;
		}

		for (auto& stage : stages)
			InitializeUniforms(stage.second, uniformInfos, uniformLocations, firstTextureUnit);
	}

	GLuint Shader::GenerateShader(const GLenum type, const char* source) const
	{
		assert(source);

		if (source[0] == '\0')
			return 0;

		GLuint shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);

		if (Validate(shaderId, ObjectType::Shader) == false)
		{
			glDeleteShader(shaderId);
			shaderId = 0;
		}

		return shaderId;
	}

	bool Shader::Validate(GLuint objId, ObjectType objType) const
	{
		GLint isSuccess;
		char errorLog[1024];

		if (objType == ObjectType::Program)
		{
			glGetProgramiv(objId, GL_LINK_STATUS, &isSuccess);

			// TODO: make logs

			if (isSuccess == 0)
			{
				glGetProgramInfoLog(objId, sizeof(errorLog), nullptr, errorLog);
				VT_LOG(Warning, "Shader program link error [program id %і]", objId);
			}
		}
		else
		{
			glGetShaderiv(objId, GL_COMPILE_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetShaderInfoLog(objId, sizeof(errorLog), nullptr, errorLog);
				VT_LOG(Warning, "Shader compile error [shader id %uі]", objId);
			}
		}

		return isSuccess != 0;
	}

	std::vector<std::pair<ShaderStage, std::string>> Shader::ConvertToGLSources(const std::string& source)
	{
		std::vector<std::pair<ShaderStage, std::string>> resultStages;

		ShaderFormatter formatter(source);

		SetupState(formatter);

		formatter.ReplaceInputAssemly(ReplaceInputAssembly);
		formatter.ReplaceUniformSets(ReplaceUniformSets);

		const auto stages = formatter.GetStages();

		for (const auto& stage : stages)
		{
			resultStages.push_back({
				stage.first,
				ConvertToGLShader({stage.second.begin(), stage.second.end()})
				});
		}

		return resultStages;
	}

	void Shader::SetupState(ShaderFormatter& formatter)
	{
		blendState.isEnable = false;
		blendState.srcFactor = GL_SRC_ALPHA;
		blendState.dstFactor = GL_ONE_MINUS_SRC_ALPHA;
		blendState.operation = GL_FUNC_ADD;

		depthWrite = true;
		stencilWrite = false;
		cullFace = true;

		if (formatter.HasStateDefinition() == false)
			return;

		depthWrite = ShaderUtils::IsUseDepthWrite(formatter);
		stencilWrite = ShaderUtils::IsUseStensilWrite(formatter);

		if (ShaderUtils::IsUseBlend(formatter))
		{
			blendState.isEnable = true;

			SetupBlendState(formatter);
		}		
	}

	void Shader::SetupBlendState(ShaderFormatter& formatter)
	{
		ShaderUtils::SetupBlendFactor(blendState, formatter);
		ShaderUtils::SetupBlendOperation(blendState, formatter);
	}

	void Shader::InitializeUniforms(std::string& source, std::vector<UniformInfo>& uniformInfos, std::vector<GLint>& uniformLocations, GLint& firstTextureUnit)
	{
		const std::regex pattern("(\\[input_attachment\\])?\\\s+uniform\\s+(\\S+)\\s+(\\S+);");
		std::smatch matches;

		GLint textureUnit = 0;

		// тут юзать ShaderUniformsInfo, поулчать их в материале, и в материале оборачивать из уже с Varlet::Core::Type ом, вместо ShaderDataType

		while (std::regex_search(source, matches, pattern))
		{
			const std::string uniformName = matches[3];
			const std::string typeName = matches[2];

			const GLint location = glGetUniformLocation(id, uniformName.c_str());

			const bool isTexture = typeName == "sampler2D" || typeName == "samplerCube";
			const bool isMaterialUniform = matches[1].matched == false;

			if (isTexture)
			{
				if (firstTextureUnit != -1 && isMaterialUniform)
					firstTextureUnit = textureUnit;

				glProgramUniform1i(id, location, textureUnit);
				++textureUnit;
			}

			if (isMaterialUniform)
			{
				uniformInfos.push_back({ uniformName, TypeNameToShaderDataType(typeName) });
				uniformLocations.push_back(location);
			}

			source = matches.suffix();
		}
	}

	std::string Shader::ReplaceInputAssembly(const ShaderInputAssemblyInfo& info)
	{
		return "layout(location = " + std::to_string(info.location) + ") in " + info.typeName + " " + info.name + ";";
	}

	std::string Shader::ReplaceUniformSets(const ShaderUniformSetInfo& info)
	{
		std::string result;

		if (info.meta.empty() || info.meta == "fast")
		{
			for (auto& value : info.values)
				result += "uniform " + value.typeName + " " + value.name + ";\n";
		}
		else
		{
			const auto bufferTypeName = info.meta == "storage" ? "buffer " : "uniform ";

			result = "layout (std430, binding = " + std::to_string(UniformBuffer::GetBufferBinding(info.name)) + ") " + bufferTypeName + info.name + "\n{\n";

			for (auto& value : info.values)
				result += value.typeName + " " + value.name + ";\n";

			result += "};\n";
		}

		return result;
	}

	std::string Shader::ConvertToGLShader(std::string source)
	{
		// 4.60 NVIDIA
		//const GLubyte* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

		return "#version 460\n#extension GL_KHR_vulkan_glsl : enable\n#extension GL_EXT_gpu_shader4 : enable\n#extension GL_NV_uniform_buffer_std430_layout : enable\n" + source;
	}

	void Shader::ProcessInputAttachments(std::string& source)
	{
		const std::regex regex("\\[vshader::input_attachment\\(\\d\\)\\]\\s*");
		source = std::regex_replace(source, regex, "uniform ");
	}

	ShaderDataType Shader::TypeNameToShaderDataType(const std::string& typeName)
	{
		if (typeName == "bool")			return ShaderDataType::Bool;
		if (typeName == "int")			return ShaderDataType::Int;
		if (typeName == "uint")			return ShaderDataType::UInt;
		if (typeName == "float")		return ShaderDataType::Float;
		if (typeName == "double")		return ShaderDataType::Double;
		if (typeName == "vec2")			return ShaderDataType::Vec2;
		if (typeName == "vec3")			return ShaderDataType::Vec3;
		if (typeName == "vec4")			return ShaderDataType::Vec4;
		if (typeName == "mat2")			return ShaderDataType::Mat2;
		if (typeName == "mat3")			return ShaderDataType::Mat3;
		if (typeName == "mat4")			return ShaderDataType::Mat4;
		if (typeName == "sampler2D")	return ShaderDataType::Sampler2D;
	}

	bool ShaderUtils::IsUseBlend(ShaderFormatter& formatter)
	{
		const auto value = formatter.GetStateValues("Blend");

		return value != nullptr
			&& value->size() == 1
			&& (*value)[0] == "On";
	}

	void ShaderUtils::SetupBlendFactor(BlendState& state, ShaderFormatter& formatter)
	{
		auto args = formatter.GetStateValues("BlendFactor");

		if (args != nullptr && args->size() == 2)
		{
			state.srcFactor = ConvertStringToBlendFactor((*args)[0]);
			state.dstFactor = ConvertStringToBlendFactor((*args)[1]);
			return;
		}

		VT_LOG(Warning, "Invalid shader blend factor arguments");
	}

	void ShaderUtils::SetupBlendOperation(BlendState& state, ShaderFormatter& formatter)
	{
		const auto args = formatter.GetStateValues("BlendOp");

		if (args != nullptr && args->size() == 1)
		{
			state.operation = ConvertStringToBlendOperation((*args)[0]);
			return;
		}

		VT_LOG(Warning, "Invalid shader blend operation arguments");
	}

	bool ShaderUtils::IsUseDepthWrite(ShaderFormatter& formatter)
	{
		const auto value = formatter.GetStateValues("DepthWrite");

		return value == nullptr
			|| (value->size() == 1
			&& (*value)[0] == "On");
	}

	bool ShaderUtils::IsUseStensilWrite(ShaderFormatter& formatter)
	{
		const auto value = formatter.GetStateValues("StensilWrite");

		return value != nullptr
			&& value->size() == 1
			&& (*value)[0] == "On";
	}

	GLenum ShaderUtils::ConvertStringToBlendFactor(const std::string& value)
	{
		if (value == "Zero")					return GL_ZERO;
		if (value == "One")						return GL_ONE;
		if (value == "SrcColor")				return GL_SRC_COLOR;
		if (value == "OneMinusSrcColor")		return GL_ONE_MINUS_SRC_COLOR;
		if (value == "DstColor")				return GL_DST_COLOR;
		if (value == "OneMinusDstColor")		return GL_ONE_MINUS_DST_COLOR;
		if (value == "SrcAlpha")				return GL_SRC_ALPHA;
		if (value == "OneMinusSrcAlpha")		return GL_ONE_MINUS_SRC_ALPHA;
		if (value == "DstAlpha")				return GL_DST_ALPHA;
		if (value == "OneMinusDstAlpha")		return GL_ONE_MINUS_DST_ALPHA;
		if (value == "ConstantColor")			return GL_CONSTANT_COLOR;
		if (value == "OneMinusConstantColor")	return GL_ONE_MINUS_CONSTANT_COLOR;
		if (value == "ConstantAlpha")			return GL_CONSTANT_ALPHA;
		if (value == "OneMinusConstantAlpha")	return GL_ONE_MINUS_CONSTANT_ALPHA;

		VT_LOG(Warning, "Invalid shader state value: blend factor");
		return GL_ZERO;
	}

	GLenum ShaderUtils::ConvertStringToBlendOperation(const std::string& value)
	{
		if (value == "Add")				return GL_FUNC_ADD;
		if (value == "Subtract")		return GL_FUNC_SUBTRACT;
		if (value == "ReverceSubtract")	return GL_FUNC_REVERSE_SUBTRACT;
		if (value == "Min")				return GL_MIN;
		if (value == "Max")				return GL_MAX;

		VT_LOG(Warning, "Invalid shader state value: blend operation");
		return GL_FUNC_ADD;
	}
}
