#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

class Component;
class MeshRenderer;
class Transform;
class Camera;	

namespace Varlet
{
	class Entity;
	class UniformBuffer;

	struct RendererData
	{
		MeshRenderer* meshRenderer;
		Transform* transform;
	};

	class Renderer : public Module, public IUpdatebleModule
	{
	protected:

		std::vector<Camera*> _cameras;
		std::vector<RendererData> _rendererData;

		UniformBuffer* _processedCameraData;

	public:

		~Renderer() override;

		virtual int32_t Init() override;

		virtual void Update() override;

	protected:

		virtual void Render(const RendererData& rendererData) = 0;

	private:

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
