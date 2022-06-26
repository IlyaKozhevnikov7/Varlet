#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

class Component;
class MeshRenderer;
class Transform;

namespace Varlet
{
	class Entity;

	struct RendererData
	{
	public:

		MeshRenderer* meshRenderer;
		Transform* transform;
	};

	class Renderer : public Module, public IUpdatebleModule
	{
	private:

		std::vector<RendererData> _rendererData;

	public:

		~Renderer() override = default;

		virtual int32_t Init() override;

	private:

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
