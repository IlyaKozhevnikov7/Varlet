#include "Renderer.h"
#include "Material.h"

START_PROPERTY_BLOCK(Renderer);
PROPERTY("Is Visible", isVisible);
ARRAY("Materials", materials);
END_PROPERTY_BLOCK

int32_t Renderer::_idCounter = 1;

void Renderer::OnConstructed()
{
	BaseClass::OnConstructed();

	isVisible = true;
	_renderId = _idCounter++;
}

const int32_t& Renderer::GetRenderId() const
{
	return _renderId;
}

glm::mat4 Renderer::GetModelMatrix()
{
	const Transform* transfrom = GetOwner()->GetComponent<Transform>();
	
	glm::mat4 model = glm::translate(glm::mat4(1.f), transfrom->position);
	model *= glm::mat4_cast(transfrom->GetOrientation());
	model = glm::scale(model, transfrom->scale);

	return model;
}

void Renderer::OnPreRender()
{
}
