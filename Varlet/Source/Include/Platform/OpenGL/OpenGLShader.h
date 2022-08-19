#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLShader final : public Shader
	{
		enum ShaderType : uint32_t
		{
			Vertex = 0x8B31,
			Fragment = 0x8B30,
			Geometry = 0x8DD9
		};

		enum class ObjectType : uint8_t
		{
			VertexShader = 0,
			FragmentShader,
			GeomertyShader,
			Program
		};

	private:

		std::unordered_map<const char*, int32_t> _uniformLocations;
		uint32_t _id;

	public:

		OpenGLShader(const ShaderInitializer& initializer);

		~OpenGLShader() override;

		void Use() const override;

		void SetBool(const char* name, const bool& value) override;

		void SetUInt32(const char* name, const uint32_t& value) override;

		void SetInt32(const char* name, const int32_t& value) override;

		void SetFloat(const char* name, const float& value) override;

		void SetVec2(const char* name, const glm::vec2& value) override;

		void SetVec3(const char* name, const glm::vec3& value) override;

		void SetVec4(const char* name, const glm::vec4& value) override;

		void SetMat3(const char* name, const glm::mat3& value) override;

		void SetMat4(const char* name, const glm::mat4& value) override;

	private:

		void Compile(const uint32_t& objId, const ObjectType& objType) const;

		std::string Load(const char* path) const;

		const uint32_t GenerateShader(const ShaderType&& type, const char* source) const;

		void TryAttach(const uint32_t& id) const;

		void SetupUniforms(const std::string& source);
	};
}
