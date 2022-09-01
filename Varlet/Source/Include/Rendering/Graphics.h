#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

class Entity;
class Component;
class MeshRenderer;
class TextureRenderer;
class Renderer;
class Transform;
class Camera;
class PointLight;

namespace Varlet
{
	class UniformBuffer;

	struct RendererData
	{
		Renderer* renderer;
		Transform* transform;
	};

	struct LightSourceData
	{
		std::vector<std::pair<PointLight*, Transform*>> pointLights;
		// Directional lights
		// Spotlights
	};

	class Graphics : public Module, public IUpdatebleModule
	{
	protected:

		std::vector<Camera*> _cameras;
		std::vector<RendererData> _rendererData;
		LightSourceData _lightSources;

		UniformBuffer* _globalData;
		UniformBuffer* _illuminationData;

	public:

		~Graphics() override;

		virtual int32_t Init() override;

		virtual void Update() override;

	private:

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
