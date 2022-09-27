#include "RenderingCore.h"

#include "Entity.h"
#include "Component.h"
#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "PointLight.h"

const uint8_t* GraphicsInfo::rendererName;
double GraphicsInfo::renderTime;

namespace Varlet
{
	Graphics::~Graphics()
	{
		delete _globalData;
		delete _illuminationData;
	}

	int32_t Graphics::Init()
	{
		Entity::NewComponentCreatedEvent.Bind(this, &Graphics::OnNewComponentCreated);

		if (auto rendererAPIInitializer = dynamic_cast<IRendererAPIInitializerBase*>(this))
		{
			rendererAPIInitializer->InitRendererAPI();
			_globalData = RendererAPI::CreateUniformBuffer(
				sizeof(glm::mat4) * 4 + // view | projection | projection-view | model
				sizeof(glm::vec3)); // camera position

			_illuminationData = RendererAPI::CreateUniformBuffer(
				(sizeof(int32_t) + sizeof(glm::vec4) * 2 + sizeof(float) * 2 + sizeof(int32_t)) * 10 // point light sizeof(isActive + position + color + linear attenuation + quadratic attenuation + padding 4 bytes) * amount (now 10)
			);

			return SUCCESSFUL_INITIALIZATION;
		}
		
		return FAILED_INITIALIZATION;
	}

	void Graphics::OnNewComponentCreated(Entity* entity, Component* ñomponent)
	{
		if (const auto renderer = dynamic_cast<Renderer*>(ñomponent))
		{
			assert(entity->HasComponent<Transform>());
			_rendererData.push_back({ renderer, entity->GetComponent<Transform>() });
			VARLET_LOG(LevelType::Normal, "Entity added new renderer");
			return;
		}

		if (const auto camera = dynamic_cast<Camera*>(ñomponent))
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
