#pragma once

#include "VarletCore.h"
#include "Rendering/Shader.h"

namespace Varlet
{
	class Texture;
}

namespace Varlet::OpenGL
{
	enum ShaderType : uint32_t
	{
		Vertex = 0x8B31,
		Fragment = 0x8B30,
		Geometry = 0x8DD9
	};

	enum ShaderBit : uint32_t
	{
		VertexShaderBit = 0x00000001,
		FragmentShaderBit = 0x00000002,
		GeometryShaderBit = 0x00000004,
		TessControlShaderBit = 0x00000008,
		TessEvaluationShaderBit = 0x00000010,
		ComputeShaderBit = 0x00000020,
		AllShaderBits = 0xFFFFFFFF
	};

	class Shader final : public Varlet::Shader
	{
		enum class ObjectType : uint8_t
		{
			Shader = 0,
			Program
		};

		struct UniformInfo final
		{
			uint32_t shaderId;
			int32_t location;
		};

	private:

		static std::unordered_map<ShaderInitializer, Shader*> _loaded;

		std::unordered_map<std::string, UniformInfo> _uniformLocations;
		std::unordered_map<std::string, int32_t> _textureUtits;
		std::unordered_map<uint32_t, uint32_t> _programs;
		uint32_t _shaderBits;

	public:

		Shader(const ShaderInitializer& initializer);

		~Shader() override;

		void SetBool(const char* name, const bool& value) override;

		void SetUInt32(const char* name, const uint32_t& value) override;

		void SetInt32(const char* name, const int32_t& value) override;

		void SetFloat(const char* name, const float& value) override;

		void SetVec2(const char* name, const glm::vec2& value) override;

		void SetVec3(const char* name, const glm::vec3& value) override;

		void SetVec4(const char* name, const glm::vec4& value) override;

		void SetMat3(const char* name, const glm::mat3& value) override;

		void SetMat4(const char* name, const glm::mat4& value) override;

		void SetTexture(const char* name, const Varlet::Texture* texture) override;

		// Internal API

		uint32_t GetShaderBits() const;

		uint32_t GetProgram(const uint32_t& bit);

	private:

		void Validate(const uint32_t& objId, const ObjectType& objType) const;

		std::string Load(const char* path) const;

		const uint32_t GenerateShader(const ShaderType&& type, const char* source) const;

		void GenerateProgram(const uint32_t& shaderId, const ShaderBit& bit);

		void SetupUniforms(const std::string& source, const uint32_t& id);
	};

	struct ShaderCache final
	{
	private:
	
		static std::unordered_map<int32_t, Shader*> cache;
	
	public:
	
		static void Add(Shader* shader);
	
		static Shader* Get(const Varlet::Shader* shader);
	};
}
