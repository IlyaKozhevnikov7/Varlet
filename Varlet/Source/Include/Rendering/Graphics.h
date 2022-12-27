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

struct CORE_API GraphicsInfo
{
	static const uint8_t* rendererName;
	static double renderTime;
};

namespace Varlet
{
	class CORE_API Graphics : public Module, public IUpdatebleModule
	{
	public:

		~Graphics() override = default;

		virtual int32_t Init() override;
	};
}
