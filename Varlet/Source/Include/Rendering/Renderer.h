#pragma once

#include "VarletCore.h"
#include "VarletModules.h"
#include "RenderingCore.h"

class Component;
class MeshRenderer;
class Transform;
class Camera;

namespace Varlet
{
	class Entity;

	struct RendererData
	{
		MeshRenderer* meshRenderer;
		Transform* transform;
	};

	class Renderer : public Module, public IUpdatebleModule
	{
	private:

		std::vector<Camera*> _cameras;
		std::vector<RendererData> _rendererData;

		// TODO uniform buffer for current processed camera

	public:

		~Renderer() override = default;

		virtual int32_t Init() override;

		virtual void Update() override;

	protected:

		virtual void Render(const RendererData& rendererData) = 0;

	private:

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
