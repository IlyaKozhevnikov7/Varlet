#include "OpenGLShader.h"
#include <regex>
#include <fstream>
#include <sstream>

#include "Core/Reflection.h"

#include "glad/glad.h"

namespace Varlet
{
	static std::unordered_map<std::string, Type> _types =
	{
		{ "bool",			Type::Bool },
		{ "int",			Type::Int32 },
		{ "uint",			Type::UInt32 },
		{ "float",			Type::Float },
		{ "double",			Type::Double },
		{ "bvec2",			Type::BoolVector2 },
		{ "bvec3",			Type::BoolVector3 },
		{ "bvec4",			Type::BoolVector4 },
		{ "ivec2",			Type::Int32Vector2 },
		{ "ivec3",			Type::Int32Vector3 },
		{ "ivec4",			Type::Int32Vector4 },
		{ "uvec2",			Type::UInt32Vector2 },
		{ "uvec3",			Type::UInt32Vector3 },
		{ "uvec4",			Type::UInt32Vector4 },
		{ "vec2",			Type::Vector2 },
		{ "vec3",			Type::Color3 },
		{ "vec4",			Type::Color4 },
		{ "dvec2",			Type::DoubleVector2 },
		{ "dvec3",			Type::DoubleVector3 },
		{ "dvec4",			Type::DoubleVector4 },
		{ "mat2",			Type::Matrix2 },
		{ "mat3",			Type::Matrix3 },
		{ "mat4",			Type::Matrix4 },
		{ "sampler2D",		Type::Sampler2D },
		{ "samplerCube",	Type::SamplerCube }
	};

	OpenGLShader::OpenGLShader(const ShaderInitializer& initializer)
	{
		_shaderBits = 0x00000000;

		std::string vertexShaderSource = Load(initializer.vertexPath.c_str());
		std::string fragmentShaderSource = Load(initializer.fragmentPath.c_str());
		std::string geomtryShaderSource = Load(initializer.geomtryPath.c_str());

		const uint32_t vertexShaderId = GenerateShader(ShaderType::Vertex, vertexShaderSource.c_str());
		const uint32_t fragmentShaderId = GenerateShader(ShaderType::Fragment, fragmentShaderSource.c_str());
		const uint32_t geomtryShaderId = GenerateShader(ShaderType::Geometry, geomtryShaderSource.c_str());

		GenerateProgram(vertexShaderId, ShaderBit::VertexShaderBit);
		GenerateProgram(fragmentShaderId, ShaderBit::FragmentShaderBit);
		GenerateProgram(geomtryShaderId, ShaderBit::GeometryShaderBit);

		SetupUniforms(vertexShaderSource, _programs[ShaderBit::VertexShaderBit]);
		SetupUniforms(fragmentShaderSource, _programs[ShaderBit::FragmentShaderBit]);
		SetupUniforms(geomtryShaderSource, _programs[ShaderBit::GeometryShaderBit]);
	}

	OpenGLShader::~OpenGLShader()
	{
		for (auto& program : _programs)
			glDeleteProgram(program.second);
	}

	void OpenGLShader::Use() const
	{
	}

	void OpenGLShader::SetBool(const char* name, const bool& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform1i(_uniformLocations[name].shaderId, _uniformLocations[name].location, static_cast<int32_t>(value));
	}

	void OpenGLShader::SetUInt32(const char* name, const uint32_t& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform1ui(_uniformLocations[name].shaderId, _uniformLocations[name].location, value);
	}

	void OpenGLShader::SetInt32(const char* name, const int32_t& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform1i(_uniformLocations[name].shaderId, _uniformLocations[name].location, value);
	}

	void OpenGLShader::SetFloat(const char* name, const float& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform1f(_uniformLocations[name].shaderId, _uniformLocations[name].location, value);
	}

	void OpenGLShader::SetVec2(const char* name, const glm::vec2& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform2f(_uniformLocations[name].shaderId, _uniformLocations[name].location, value.x, value.y);
	}

	void OpenGLShader::SetVec3(const char* name, const glm::vec3& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform3f(_uniformLocations[name].shaderId, _uniformLocations[name].location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetVec4(const char* name, const glm::vec4& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniform4f(_uniformLocations[name].shaderId, _uniformLocations[name].location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const char* name, const glm::mat3& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniformMatrix3fv(_uniformLocations[name].shaderId, _uniformLocations[name].location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& value)
	{
		if (_uniformLocations.contains(name))
			glProgramUniformMatrix4fv(_uniformLocations[name].shaderId, _uniformLocations[name].location, 1, GL_FALSE, glm::value_ptr(value));
	}

	uint32_t OpenGLShader::GetShaderBits() const
	{
		return _shaderBits;
	}

	uint32_t OpenGLShader::GetProgram(const uint32_t& bit)
	{
		return _programs[bit];
	}

	void OpenGLShader::Validate(const uint32_t& objId, const ObjectType& objType) const
	{
		int32_t isSuccess;
		char errorLog[512];

		if (objType == ObjectType::Program)
		{
			glGetProgramiv(objId, GL_LINK_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetProgramInfoLog(objId, sizeof(errorLog), NULL, errorLog);
				VARLET_LOG(LevelType::Warning, "Shader program link error [program id " + std::to_string(objId) + "] " + errorLog);
			}
		}
		else
		{
			glGetShaderiv(objId, GL_COMPILE_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetShaderInfoLog(objId, sizeof(errorLog), NULL, errorLog);
				VARLET_LOG(LevelType::Warning, "Shader compile error [shader id " + std::to_string(objId) + "] " + errorLog);
			}
		}
	}

	std::string OpenGLShader::Load(const char* path) const
	{
		std::ifstream stream;
		stream.open(path);

		if (stream.is_open())
		{
			std::stringstream buffer;
			buffer << stream.rdbuf();
			return buffer.str();
		}
		else
		{
			VARLET_LOG(LevelType::Warning, "Failed load shader: " + std::string(path));
			return std::string();
		}
	}

	const uint32_t OpenGLShader::GenerateShader(const ShaderType&& type, const char* source) const
	{
		if (source[0] == '\0')
			return 0;

		const uint32_t vertexShaderId = glCreateShader(type);
		glShaderSource(vertexShaderId, 1, &source, NULL);
		glCompileShader(vertexShaderId);

		Validate(vertexShaderId, ObjectType::Shader);

		return vertexShaderId;
	}

	void OpenGLShader::GenerateProgram(const uint32_t& shaderId, const ShaderBit& bit)
	{
		if (shaderId == 0)
			return;

		const uint32_t id = glCreateProgram();
		glProgramParameteri(id, GL_PROGRAM_SEPARABLE, GL_TRUE);

		glAttachShader(id, shaderId);
		glLinkProgram(id);

		Validate(id, ObjectType::Program);

		glDetachShader(id, shaderId);
		glDeleteShader(shaderId);

		_programs[bit] = id;
		_shaderBits |= bit;
	}

	void OpenGLShader::SetupUniforms(const std::string& source, const uint32_t& id)
	{
		if (id == 0)
			return;

		const std::regex reg("uniform\\s+(bool|int|uint|float|double|bvec2|bvec3|bvec4|ivec2|ivec3|ivec4|uvec2|uvec3|uvec4|vec2|vec3|vec4|dvec2|dvec3|dvec4|mat2|mat3|mat4|samplerCube|sampler2D)\\s+(\\w*)", std::regex_constants::ECMAScript);
		std::smatch matches;
		std::string suffix = source;
		int32_t textureUnit = 0;

		while (std::regex_search(suffix, matches, reg))
		{
			const char* typeName = strdup(matches[1].str().c_str());
			const char* uniformName = strdup(matches[2].str().c_str());

			if (_types.contains(typeName))
				_uniformDeclarations.push_back({ uniformName, _types[typeName] });

			const int32_t location = glGetUniformLocation(id, uniformName);
			if (location != -1)
			{
				_uniformLocations[uniformName] = { id, location };
			}
			else
			{
				VARLET_LOG(LevelType::Warning, "Uniform variable " + *uniformName + *" is not used");
			}

			if (!strcmp(typeName, "sampler2D") || !strcmp(typeName, "samplerCube"))
			{
				glProgramUniform1i(id, location, textureUnit);
				textureUnit++;
			}

			suffix = matches.suffix();
		}
	}

	// -------------------- OpenGLShaderCache --------------------

	std::unordered_map<size_t, OpenGLShader*> OpenGLShaderCache::cache;

	void OpenGLShaderCache::Add(OpenGLShader* shader)
	{
		const size_t address = reinterpret_cast<size_t>(shader);
		assert(cache.contains(address) == false);

		cache[address] = shader;
	}

	OpenGLShader* OpenGLShaderCache::Get(const Shader* shader)
	{
		const size_t address = reinterpret_cast<size_t>(shader);

		if (cache.contains(address))
			return cache[address];

		return nullptr;
	}


}
