#include "Components/Renderer.h"
#include "Components/Transform.h"

Event<Renderer*> Renderer::OnRendererCreated;
Event<Renderer*> Renderer::OnRendererDestroyed;

void Renderer::Start()
{
	OnRendererCreated.Invoke(this);

	_transfrom = GetOwner()->GetComponent<Transform>();
	assert(_transfrom);
}

void Renderer::OnDestroyed()
{
	OnRendererDestroyed.Invoke(this);
}

Mesh* Renderer::GetMesh() const
{
	return nullptr;
}

glm::mat4 Renderer::GetModelMatrix()
{	
	glm::mat4 model = glm::translate(glm::mat4(1.f), _transfrom->position);
	model *= glm::mat4_cast(_transfrom->GetOrientation());
	model = glm::scale(model, _transfrom->scale);

	return model;
}

void Renderer::OnPreRender()
{
}
