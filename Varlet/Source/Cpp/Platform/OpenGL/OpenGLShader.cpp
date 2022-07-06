#include "OpenGLShader.h"
#include <fstream>
#include <sstream>

#include "glad/glad.h"

namespace Varlet
{
	OpenGLShader::OpenGLShader(const ShaderInitializer& initializer) : Shader(initializer)
	{
		std::string vertexShaderSource = Load(initializer.vertexPath.c_str());
		std::string fragmentShaderSource = Load(initializer.fragmentPath.c_str());
		std::string geomtryShaderSource = Load(initializer.geomtryPath.c_str());

		uint32_t vertexShaderId = GenerateShader(ShaderType::Vertex, vertexShaderSource.c_str());
		uint32_t fragmentShaderId = GenerateShader(ShaderType::Fragment, fragmentShaderSource.c_str());
		uint32_t geomtryShaderId = GenerateShader(ShaderType::Geometry, geomtryShaderSource.c_str());

		_id = glCreateProgram();

		TryAttach(vertexShaderId);
		TryAttach(fragmentShaderId);
		TryAttach(geomtryShaderId);

		glLinkProgram(_id);
		Compile(_id, ObjectType::Program);

		if (vertexShaderId != 0)
			glDeleteProgram(vertexShaderId);

		if (fragmentShaderId != 0)
			glDeleteProgram(fragmentShaderId);

		if (geomtryShaderId == 0)
			glDeleteProgram(geomtryShaderId);
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
				glGetProgramInfoLog(objId, 512, NULL, errorLog);
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
			VARLET_LOG(LevelType::Warning, "Failed load shader: " + *path);
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
		glUniform1i(glGetUniformLocation(_id, name), static_cast<int32_t>(value));
	}

	void OpenGLShader::SetUInt32(const char* name, const uint32_t& value)
	{
		glUniform1ui(glGetUniformLocation(_id, name), value);
	}

	void OpenGLShader::SetInt32(const char* name, const int32_t& value)
	{
		glUniform1i(glGetUniformLocation(_id, name), value);
	}

	void OpenGLShader::SetFloat(const char* name, const float& value)
	{
		glUniform1f(glGetUniformLocation(_id, name), value);
	}

	void OpenGLShader::SetVec2(const char* name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(_id, name), value.x, value.y);
	}

	void OpenGLShader::SetVec3(const char* name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(_id, name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetVec4(const char* name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(_id, name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const char* name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
	}
}
