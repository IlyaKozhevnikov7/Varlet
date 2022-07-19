#pragma once

#include "VarletCore.h"

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

struct CORE_API MaterialSettings
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
};

namespace Varlet
{
	class Shader;
}

class CORE_API Material
{
public:

	bool isActive;
	MaterialSettings settings;

private:

	const Varlet::Shader* _shader;

public:

	Material(const  Varlet::Shader* shader);

	void Activate() const;

	// TODO remove
	Varlet::Shader* GetShader();
};

