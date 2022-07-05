#include "Renderer.h"
#include "RendererAPI.h"
#include "RenderingCore.h"

#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

namespace Varlet
{
	Renderer::~Renderer()
	{
		delete _processedCameraData;
	}

	int32_t Renderer::Init()
	{
		Entity::NewComponentCreatedEvent.Bind(this, &Renderer::OnNewComponentCreated);

		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			_processedCameraData = RendererAPI::CreateUniformBuffer(sizeof(glm::mat4) * 3); // view, projection, and view-projection

			return SUCCESSFUL_INITIALIZATION;
		}

		return FAILED_INITIALIZATION;
	}

	void Renderer::Update()
	{
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
