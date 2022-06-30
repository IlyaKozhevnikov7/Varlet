#include "OpenGLShader.h"

#include "glad/glad.h"

namespace Varlet
{
	OpenGLShader::OpenGLShader(const ShaderInitializer& initializer) : Shader(initializer)
	{
		// now here automaticly created simple triangle shader
		const char* vertexShaderSource = R"(
		#version 460 core

		layout (location = 0) in vec2 aPos;

		void main()
		{
			gl_Position = vec4(aPos, 0.f, 1.f);
		})";

		const char* geomtryShaderSource = R"(
		#version 460 core

		layout (triangle_strip, max_vertices = 3) out;

		out vec3 color;

		void main()
		{
			color = vec3(1.f, 0.f, 0.f);
			gl_Position = vec4(-0.5f, -0.5f, 0.f, 0.f);
			EmitVertex();

			color = vec3(0.f, 1.f, 0.f);
			gl_Position = vec4(0.f, 0.5f, 0.f, 0.f);
			EmitVertex();

			color = vec3(0.f, 0.f, 1.f);
			gl_Position = vec4(0.5f, -0.5f, 0.f, 0.f);
			EmitVertex();

			EndPrimitive();
		})";

		const char* fragmentShaderSource = R"(
		#version 460 core

		out vec4 fragColor;

		void main()
		{
			fragColor = vec4(1.f, 0.5f, 0.f, 1.f);
		})";

		uint32_t vertexShaderId = glCreateShader(ShaderType::Vertex);
		glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShaderId);
		Compile(vertexShaderId, ObjectType::VertexShader);

		uint32_t geomtryShaderId = glCreateShader(ShaderType::Geometry);
		glShaderSource(geomtryShaderId, 1, &geomtryShaderSource, NULL);
		glCompileShader(geomtryShaderId);
		Compile(geomtryShaderId, ObjectType::GeomertyShader);

		uint32_t fragmentShaderId = glCreateShader(ShaderType::Fragment);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShaderId);
		Compile(fragmentShaderId, ObjectType::FragmentShader);

		_id = glCreateProgram();
		glAttachShader(_id, vertexShaderId);
		glAttachShader(_id, geomtryShaderId);
		glAttachShader(_id, fragmentShaderId);
		glLinkProgram(_id);
		Compile(_id, ObjectType::Program);

		glDeleteProgram(vertexShaderId);
		glDeleteProgram(geomtryShaderId);
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

	void OpenGLShader::Compile(const uint32_t& objId, ObjectType&& objType)
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
}
