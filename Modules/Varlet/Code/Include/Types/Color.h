#pragma once

#include "VarletCore.h"
#include "Color.generated.h"

REFLECTABLE()
struct VARLET_API Color final
{
	GENERATION

public:

	FIELD()
	float r;

	FIELD()
	float g;

	FIELD()
	float b;

	static const Color White;

public:

	Color();

	Color(float r, float g, float b);
};
