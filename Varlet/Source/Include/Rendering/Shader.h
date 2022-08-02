#pragma once

#include "VarletCore.h"

namespace Varlet
{
	struct CORE_API ShaderInitializer final
	{
	public:

		std::string vertexPath;
		std::string fragmentPath;
		std::string geomtryPath;
	};

	class CORE_API Shader
	{
	public:

		Shader(const ShaderInitializer& initializer);

		virtual ~Shader() = default;

		virtual void Use() const = 0;

		virtual void SetBool(const char* name, const bool& value) = 0;

		virtual void SetUInt32(const char* name, const uint32_t& value) = 0;

		virtual void SetInt32(const char* name, const int32_t& value) = 0;

		virtual void SetFloat(const char* name, const float& value) = 0;

		virtual void SetVec2(const char* name, const glm::vec2& value) = 0;

		virtual void SetVec3(const char* name, const glm::vec3& value) = 0;

		virtual void SetVec4(const char* name, const glm::vec4& value) = 0;

		virtual void SetMat3(const char* name, const glm::mat3& value) = 0;

		virtual void SetMat4(const char* name, const glm::mat4& value) = 0;
	};
}