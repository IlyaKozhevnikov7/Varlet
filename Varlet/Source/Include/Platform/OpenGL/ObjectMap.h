#pragma once

#include <unordered_map>
#include "Scene/Camera.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"

namespace Varlet::OpenGL
{
	struct Camera;

	struct ObjectMap final
	{
		using CameraComponent = ::Camera;

	private:

		static std::unordered_map<CameraComponent*, Camera*> _cameras;
		static std::unordered_map<const Varlet::Texture*, Texture*> _textures;

	public:

		static const std::unordered_map<CameraComponent*, Camera*>& GetCameras();

		static Camera* GetCamera(CameraComponent* camera);

		static uint32_t* GetTexture(const Varlet::Texture* texture);

		template<typename T>
		static void Register(T object)
		{
			VARLET_ERROR("OnNewObjectCreated error");
		}

		template<>
		static void Register<CameraComponent*>(CameraComponent* object)
		{
			int32_t width, height;
			object->GetResolution(width, height);
			_cameras[object] = new Camera(width, height);
		}

		template<>
		static void Register<Texture*>(Texture* object)
		{
			_textures[object] = object;
		}
	};
}