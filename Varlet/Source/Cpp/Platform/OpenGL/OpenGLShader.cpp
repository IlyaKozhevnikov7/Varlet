#include "OpenGLShader.h"
#include <fstream>
#include <sstream>

#include "glad/glad.h"

namespace Varlet
{
	OpenGLShader::OpenGLShader(const ShaderInitializer& initializer) : Shader(initializer)
	{
		const char* vertexShaderSource = Load(initializer.vertexPath.c_str());
		const char* fragmentShaderSource = Load(initializer.vertexPath.c_str());
		const char* geomtryShaderSource = Load(initializer.geomtryPath.c_str());

		uint32_t vertexShaderId = GenerateShader(ShaderType::Vertex, vertexShaderSource);
		uint32_t fragmentShaderId = GenerateShader(ShaderType::Fragment, fragmentShaderSource);
		uint32_t geomtryShaderId = GenerateShader(ShaderType::Geometry, geomtryShaderSource);

		_id = glCreateProgram();

		TryAttach(vertexShaderId);
		TryAttach(fragmentShaderId);
		TryAttach(geomtryShaderId);

		glLinkProgram(_id);
		Compile(_id, ObjectType::Program);

		if (vertexShaderId == 0)
			glDeleteProgram(vertexShaderId);

		if (geomtryShaderId == 0)
			glDeleteProgram(geomtryShaderId);

		if (fragmentShaderId == 0)
			glDeleteProgram(fragmentShaderId);
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

	const char* OpenGLShader::Load(const char* path) const
	{
		std::ifstream stream;
		stream.open(path);

		if (stream.is_open())
		{
			std::stringstream buffer;
			buffer << stream.rdbuf();
			return buffer.str().c_str();
		}
		else
		{
			VARLET_LOG(LevelType::Warning, "Failed load shader: " + *path);
			return nullptr;
		}
	}

	const uint32_t OpenGLShader::GenerateShader(const ShaderType&& type, const char* source) const
	{
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
}
