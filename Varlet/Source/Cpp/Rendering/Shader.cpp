#include "Rendering/Shader.h"

namespace Varlet
{
    uint32_t Shader::_idCounter = 1;

    Shader::Shader()
    {
        _id = _idCounter;
        ++_idCounter;
    }

    Shader* Shader::Create(const ShaderInitializer& initializer)
    {


        return nullptr;
    }

    uint32_t Shader::GetId() const
    {
        return _id;
    }
}
