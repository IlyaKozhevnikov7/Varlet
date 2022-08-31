#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

class Entity;
class Component;
class MeshRenderer;
class Transform;
class Camera;
class PointLight;

namespace Varlet
{
	class UniformBuffer;

	struct RendererData
	{
		MeshRenderer* meshRenderer;
		Transform* transform;
	};

	struct LightSourceData
	{
		std::vector<std::pair<PointLight*, Transform*>> pointLights;
		// Directional lights
		// Spotlights
	};

	class Renderer : public Module, public IUpdatebleModule
	{
	protected:

		std::vector<Camera*> _cameras;
		std::vector<RendererData> _rendererData;
		LightSourceData _lightSources;

		UniformBuffer* _globalData;
		UniformBuffer* _illuminationData;

	public:

		~Renderer() override;

		virtual int32_t Init() override;

		virtual void Update() override;

	private:

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
