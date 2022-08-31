#include "PointLight.h"

START_PROPERTY_BLOCK(PointLight)
TYPED_PROPERTY("Color", color, Varlet::Type::Color3);
PROPERTY("Linear", linear);
PROPERTY("Quadratic", quadratic);
END_PROPERTY_BLOCK

void PointLight::OnConstructed()
{
	BaseClass::OnConstructed();

	color = glm::vec3(1.f);
	linear = 0.f;
	quadratic = 0.f;
}
