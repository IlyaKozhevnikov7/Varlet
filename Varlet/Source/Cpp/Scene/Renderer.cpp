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

void Renderer::OnPreRender()
{
}
