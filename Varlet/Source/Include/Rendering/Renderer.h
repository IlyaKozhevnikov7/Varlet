#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

class Component;
class MeshRenderer;

namespace Varlet
{
	class Entity;

	class Renderer : public Module, public IUpdatebleModule
	{
	private:

		std::vector<MeshRenderer*> _meshRenderers;

	public:

		~Renderer() override = default;

		virtual int32_t Init() override;

	private:

		void OnNewComponentCreated(Entity* entity, Component* ñomponent);
	};
}
