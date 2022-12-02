#pragma once

#include "VarletCore.h"

class Material;

struct ShaderInitializer final
{
public:

	std::string vertexPath;
	std::string fragmentPath;
	std::string geomtryPath;
};

namespace Varlet
{
	enum class Type : uint8_t;

	class Texture;

	class CORE_API Shader
	{
		friend class Material;

	private:

		static uint32_t _idCounter;
		uint32_t _id;

	protected:

		std::vector<std::pair<const char*, Varlet::Type>> _uniformDeclarations;

	protected:

		Shader() = default;

	public:

		static Shader* Create(const ShaderInitializer& initializer);

		virtual ~Shader() = default;

		uint32_t GetId() const;

		virtual void SetBool(const char* name, const bool& value) = 0;

		virtual void SetUInt32(const char* name, const uint32_t& value) = 0;

		virtual void SetInt32(const char* name, const int32_t& value) = 0;

		virtual void SetFloat(const char* name, const float& value) = 0;

		virtual void SetVec2(const char* name, const glm::vec2& value) = 0;

		virtual void SetVec3(const char* name, const glm::vec3& value) = 0;

		virtual void SetVec4(const char* name, const glm::vec4& value) = 0;

		virtual void SetMat3(const char* name, const glm::mat3& value) = 0;

		virtual void SetTexture(const char* name, const Texture* texture) = 0;

		virtual void SetMat4(const char* name, const glm::mat4& value) = 0;
	};
}
