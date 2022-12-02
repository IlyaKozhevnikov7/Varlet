#include "OpenGL/ObjectMap.h"

namespace Varlet::OpenGL
{
    std::unordered_map<ObjectMap::CameraComponent*, Camera*> ObjectMap::_cameras;
    std::unordered_map<const Varlet::Texture*, Texture*> ObjectMap::_textures;

    const std::unordered_map<ObjectMap::CameraComponent*, Camera*>& ObjectMap::GetCameras()
    {
        return _cameras;
    }

    Camera* ObjectMap::GetCamera(ObjectMap::CameraComponent* camera)
    {
        return _cameras[camera];
    }

    uint32_t* ObjectMap::GetTexture(const Varlet::Texture* texture)
    {
        Texture* nativeTexture = _textures[texture];

        return nativeTexture != nullptr
            ? &nativeTexture->id
            : nullptr;
    }
}
