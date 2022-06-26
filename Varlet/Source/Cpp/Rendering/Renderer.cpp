#include "Renderer.h"
#include "RendererAPI.h"

#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "Transform.h"

namespace Varlet
{
	int32_t Renderer::Init()
	{
		Entity::newComponentCreated.Bind(this, &Renderer::OnNewComponentCreated);

		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			return SUCCESSFUL_INITIALIZATION;
		}

		return FAILED_INITIALIZATION;
	}

	void Renderer::OnNewComponentCreated(Entity* entity, Component* ñomponent)
	{
		if (auto meshRenderer = dynamic_cast<MeshRenderer*>(ñomponent))
		{
			RendererData data;//{ meshRenderer, entity->GetComponent<Transform>() };
			data.meshRenderer = meshRenderer;
			data.transform = entity->GetComponent<Transform>();

			VARLET_LOG(LevelType::Normal, "Entity added new mesh renderer");
		}
	}
}
