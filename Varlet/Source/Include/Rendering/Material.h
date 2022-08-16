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

public:

	Material(Varlet::Shader* shader);

	void Activate() const;

	// TODO remove
	Varlet::Shader* GetShader();
};

