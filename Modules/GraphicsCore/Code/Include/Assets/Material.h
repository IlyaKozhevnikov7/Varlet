#pragma once

#include "GraphicsCoreCore.h"
#include "Assets/Shader.h"
#include "Material.generated.h"

class Texture2D;

namespace Varlet::Graphics
{
	enum class ShaderDataType : uint8_t;
}

struct MaterialUniformInfo final
{
	uint32_t size;
	uint32_t offset;
	const Varlet::Core::Type* type;
};

struct ShaderUniformLayoutInfo final
{
	uint32_t size = 0;
	std::unordered_map<std::string, MaterialUniformInfo> uniformInfos;
};

REFLECTABLE()
class GRAPHICSCORE_API Material final : public IAsset, public ICustomSerializable
{
	GENERATION

public:

	FIELD(EditorAccessible)
	bool isActive;

	FIELD()
	std::vector<int8_t> uniformBuffer;

private:
	
	static std::unordered_map<Shader*, ShaderUniformLayoutInfo> _uniformLayouts;

	FIELD()
	Shader* _shader = nullptr;

public:

	Material(Shader* shader);

	void SetShader(Shader* newShader);

	Shader* GetShader();

	void SetBool(const char* name, bool value);

	void SetUInt32(const char* name, uint32_t value);

	void SetInt32(const char* name, int32_t value);

	void SetFloat(const char* name, float value);

	void SetVec2(const char* name, const glm::vec2& value);

	void SetVec3(const char* name, const glm::vec3& value);

	void SetVec4(const char* name, const glm::vec4& value);

	void SetMat3(const char* name, const glm::mat3& value);

	void SetMat4(const char* name, const glm::mat4& value);

	void SetTexture2D(const char* name, Texture2D* value);

	const ShaderUniformLayoutInfo& GetUniformLayoutInfo() const;

	static const std::vector<int8_t>& ReceiveUniformBuffer(Material* material);

private:

	Material() = default;

	void Serialize(ISerializeContext* context) override;

	bool Deserialize(IDeserializeContext* context) override;

	uint32_t GetShaderTypeSize(Varlet::Graphics::ShaderDataType type);

	const Varlet::Core::Type* ShaderDataTypeToType(Varlet::Graphics::ShaderDataType type);

	void SetUniformValue(const char* name, const void* value);
};

