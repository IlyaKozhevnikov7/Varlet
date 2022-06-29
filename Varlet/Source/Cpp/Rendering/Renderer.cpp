#include "Renderer.h"
#include "RendererAPI.h"

#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

namespace Varlet
{
	int32_t Renderer::Init()
	{
		Entity::NewComponentCreatedEvent.Bind(this, &Renderer::OnNewComponentCreated);

		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			return SUCCESSFUL_INITIALIZATION;
		}

		return FAILED_INITIALIZATION;
	}

	void Renderer::Update()
	{
		for (const auto camera : _cameras)
		{
			if (camera->IsActive() == false)
				continue;

			// use uniform buffer for current camera

			for (const auto data : _rendererData)
				Render(data);
		}

		VARLET_LOG(LevelType::Normal, "Renderer::Update()");
	}

	void Renderer::OnNewComponentCreated(Entity* entity, Component* ñomponent)
	{
		if (auto meshRenderer = dynamic_cast<MeshRenderer*>(ñomponent))
		{
			assert(entity->HasComponent<Transform>());

			_rendererData.push_back({ meshRenderer, entity->GetComponent<Transform>() });

			VARLET_LOG(LevelType::Normal, "Entity added new mesh renderer");
		}

		if (auto camera = dynamic_cast<Camera*>(ñomponent))
		{
			_cameras.push_back(camera);

			VARLET_LOG(LevelType::Normal, "Entity added new camera");
		}
	}
}
