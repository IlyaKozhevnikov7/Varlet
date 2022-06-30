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

	int32_t Renderer::PostInit()
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
		for (const auto camera : _cameras)
		{
			if (camera->IsActive() == false)
				continue;

			// use uniform buffer for current camera
			_processedCameraData->Bind();
			_processedCameraData->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
			_processedCameraData->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjection()));
			_processedCameraData->SetData(sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjection()));

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
