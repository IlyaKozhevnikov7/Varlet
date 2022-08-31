#include "Renderer.h"
#include "RendererAPI.h"
#include "RenderingCore.h"

#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "PointLight.h"

namespace Varlet
{
	Renderer::~Renderer()
	{
		delete _globalData;
		delete _illuminationData;
	}

	int32_t Renderer::Init()
	{
		Entity::NewComponentCreatedEvent.Bind(this, &Renderer::OnNewComponentCreated);

		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			_globalData = RendererAPI::CreateUniformBuffer(
				sizeof(glm::mat4) * 4 + // view | projection | projection-view | model
				sizeof(glm::vec3) + // camera position
				sizeof(int32_t) // render id
				);

			_illuminationData = RendererAPI::CreateUniformBuffer(
				(sizeof(int32_t) + sizeof(glm::vec4) * 2 + sizeof(float) * 2 + sizeof(int32_t)) * 10 // point light sizeof(isActive + position + color + linear attenuation + quadratic attenuation + padding 4 bytes) * amount (now 10)
			);

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
			return;
		}

		if (auto camera = dynamic_cast<Camera*>(ñomponent))
		{
			assert(entity->HasComponent<Transform>());
			_cameras.push_back(camera);
			VARLET_LOG(LevelType::Normal, "Entity added new camera");
			return;
		}

		if (dynamic_cast<LightSource*>(ñomponent))
		{
			if (const auto pointLight = dynamic_cast<PointLight*>(ñomponent))
			{
				assert(entity->HasComponent<Transform>());
				_lightSources.pointLights.push_back({ pointLight, entity->GetComponent<Transform>() });
				VARLET_LOG(LevelType::Normal, "Entity added new point light");
				return;
			}
		}
	}
}
