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

class Material final : public Object
{
	TYPE_GENERATION(Material, Object)

public:

	bool isActive;
	MaterialSettings settings;

private:

	Varlet::Shader* _shader;

public:

	CORE_API Material(Varlet::Shader* shader);

	void Activate() const;

	CORE_API void SetBool(const char* name, const bool& value) const;

	CORE_API void SetUInt32(const char* name, const uint32_t& value) const;

	CORE_API void SetInt32(const char* name, const int32_t& value) const;

	CORE_API void SetFloat(const char* name, const float& value) const;

	CORE_API void SetVec2(const char* name, const glm::vec2& value) const;

	CORE_API void SetVec3(const char* name, const glm::vec3& value) const;

	CORE_API void SetVec4(const char* name, const glm::vec4& value) const;

	CORE_API void SetMat3(const char* name, const glm::mat3& value) const;

	CORE_API void SetMat4(const char* name, const glm::mat4& value) const;

private:

#ifdef META
	void SetUniform(const Uniform& uniform) const;
#endif // META
};

