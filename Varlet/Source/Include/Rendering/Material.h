#pragma once

#include "VarletCore.h"
#include "VarletFramework.h"

enum StensilFunction : uint8_t
{
	Never = 0,
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual,
	Always
};

enum StensilOp : uint8_t
{
	Keep = 0,
	Zero,
	Replace,
	Incr,
	IncrWrap,
	Decr,
	DecrWrap,
	Invert
};

struct MaterialSettings final
{
	struct
	{
		bool enable;

		uint32_t mask;
		StensilFunction function;
		int32_t ref;

		StensilOp failOp,
			zFailOp,
			allPass;

	} stencilTest;

	bool depthTest;
};

namespace Varlet
{
	class Shader;
}

class CORE_API Material final : public Object
{
	TYPE_GENERATION(Material, Object)

public:

	bool isActive;
	MaterialSettings settings;

private:

	Varlet::Shader* _shader;
	mutable std::unordered_map<std::string, Varlet::Texture*> _textures;

#ifdef META
	mutable std::vector<void*> _uniformValues;
#endif // META

public:

	Material(Varlet::Shader* shader);

	void Activate() const;

	void SetShader(Varlet::Shader* newShader);

	const Varlet::Shader* GetShader() const;

	void SetBool(const char* name, const bool& value) const;

	void SetUInt32(const char* name, const uint32_t& value) const;

	void SetInt32(const char* name, const int32_t& value) const;

	void SetFloat(const char* name, const float& value) const;

	void SetVec2(const char* name, const glm::vec2& value) const;

	void SetVec3(const char* name, const glm::vec3& value) const;

	void SetVec4(const char* name, const glm::vec4& value) const;

	void SetMat3(const char* name, const glm::mat3& value) const;

	void SetMat4(const char* name, const glm::mat4& value) const;

	void SetSampler2D(const char* name, Varlet::Texture* value) const;

private:

	void BindTextures() const;

#ifdef META
	void SetUniform(const char* name, void* value, const Varlet::Type& type) const;
#endif // META
};

