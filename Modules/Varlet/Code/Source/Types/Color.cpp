#include "Types/Color.h"

const Color Color::White = { 1.f, 1.f, 1.f };

Color::Color()
{	
	r = g = b = 0.f;
}

Color::Color(float r, float g, float b) :
	r(r),
	g(g),
	b(b)
{
}