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

	OpenGLShader::OpenGLShader(const ShaderInitializer&
		initializer)
	{
		std::string vertexShaderSource = Load(initializer.vertexPath.c_str());
		std::string fragmentShaderSource = Load(initializer.fragmentPath.c_str());
		std::string geomtryShaderSource = Load(initializer.geomtryPath.c_str());

		const uint32_t vertexShaderId = GenerateShader(ShaderType::Vertex, vertexShaderSource.c_str());
		const uint32_t fragmentShaderId = GenerateShader(ShaderType::Fragment, fragmentShaderSource.c_str());
		const uint32_t geomtryShaderId = GenerateShader(ShaderType::Geometry, geomtryShaderSource.c_str());

		_id = glCreateProgram();

		TryAttach(vertexShaderId);
		TryAttach(fragmentShaderId);
		TryAttach(geomtryShaderId);

		glLinkProgram(_id);
		Compile(_id, ObjectType::Program);

		if (vertexShaderId != 0)
			glDeleteShader(vertexShaderId);

		if (fragmentShaderId != 0)
			glDeleteShader(fragmentShaderId);

		if (geomtryShaderId != 0)
			glDeleteShader(geomtryShaderId);

		Use();
		SetupUniforms(vertexShaderSource);
		SetupUniforms(fragmentShaderSource);
		SetupUniforms(geomtryShaderSource);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_id);
	}

	void OpenGLShader::Use() const
	{
		glUseProgram(_id);
	}

	void OpenGLShader::Compile(const uint32_t& objId, const ObjectType& objType) const
	{
		int32_t isSuccess;
		char errorLog[512];

		if (objType == ObjectType::Program)
		{
			glGetProgramiv(objId, GL_LINK_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetProgramInfoLog(objId, sizeof(errorLog), NULL, errorLog);
				VARLET_LOG(LevelType::Warning, "Shader program link error [program id" + std::to_string(objId) + "] " + errorLog);
			}
		}
		else
		{
			glGetShaderiv(objId, GL_COMPILE_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetShaderInfoLog(objId, sizeof(errorLog), NULL, errorLog);
				VARLET_LOG(LevelType::Warning, "Shader compile error [shader " + std::to_string(objId) + "] " + errorLog);
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

		ObjectType objectType;

		switch (type)
		{
		case ShaderType::Vertex:
			objectType = ObjectType::VertexShader;
			break;

		case ShaderType::Fragment:
			objectType = ObjectType::FragmentShader;
			break;

		case ShaderType::Geometry:
			objectType = ObjectType::GeomertyShader;
			break;

		default:
			return 0;
		}

		Compile(vertexShaderId, objectType);

		return vertexShaderId;
	}

	void OpenGLShader::TryAttach(const uint32_t& id) const
	{
		if (id != 0)
			glAttachShader(_id, id);
	}

	void OpenGLShader::SetBool(const char* name, const bool& value)
	{
		if (_uniformLocations.contains(name))
			glUniform1i(_uniformLocations[name], static_cast<int32_t>(value));
	}

	void OpenGLShader::SetUInt32(const char* name, const uint32_t& value)
	{
		if (_uniformLocations.contains(name))
			glUniform1ui(_uniformLocations[name], value);
	}

	void OpenGLShader::SetInt32(const char* name, const int32_t& value)
	{
		if (_uniformLocations.contains(name))
			glUniform1i(_uniformLocations[name], value);
	}

	void OpenGLShader::SetFloat(const char* name, const float& value)
	{
		if (_uniformLocations.contains(name))
			glUniform1f(_uniformLocations[name], value);
	}

	void OpenGLShader::SetVec2(const char* name, const glm::vec2& value)
	{
		if (_uniformLocations.contains(name))
			glUniform2f(_uniformLocations[name], value.x, value.y);
	}

	void OpenGLShader::SetVec3(const char* name, const glm::vec3& value)
	{
		if (_uniformLocations.contains(name))
			glUniform3f(_uniformLocations[name], value.x, value.y, value.z);
	}

	void OpenGLShader::SetVec4(const char* name, const glm::vec4& value)
	{
		if (_uniformLocations.contains(name))
			glUniform4f(_uniformLocations[name], value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const char* name, const glm::mat3& value)
	{
		if (_uniformLocations.contains(name))
			glUniformMatrix3fv(_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& value)
	{
		if (_uniformLocations.contains(name))
			glUniformMatrix4fv(_uniformLocations[name], 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetupUniforms(const std::string& source)
	{
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

			const int32_t location = glGetUniformLocation(_id, uniformName);
			if (location != -1)
			{
				_uniformLocations[uniformName] = location;
			}
			else
			{
				VARLET_LOG(LevelType::Warning, "Uniform variable " + *uniformName + *" is not used");
			}

			if (!strcmp(typeName, "sampler2D") || !strcmp(typeName, "samplerCube"))
			{	
				glUniform1i(location, textureUnit);
				textureUnit++;
			}

			suffix = matches.suffix();
		}
	}
}