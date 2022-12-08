#include "Rendering/Shader.h"
#include "Rendering/RendererAPI.h"

#include <fstream>

ShaderInitializer::ShaderInitializer(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geomtryPath) :
    vertexPath(vertexPath),
    fragmentPath(fragmentPath),
    geomtryPath(geomtryPath)
{
}

namespace Varlet
{
    std::map<Shader::ShaderPath, Shader*> Shader::_loaded;
    uint32_t Shader::_idCounter = 1;

    Shader::Shader()
    {
        _id = _idCounter;
        ++_idCounter;
    }

    Shader* Shader::Create(const ShaderInitializer& initializer)
    {
        ShaderPath path = { initializer.vertexPath, initializer.fragmentPath, initializer.geomtryPath };

        if (_loaded.contains(path) == false)
        {
            const std::string vertSource = Load(initializer.vertexPath.c_str());
            const std::string fragSource = Load(initializer.fragmentPath.c_str());
            const std::string geomSource = Load(initializer.geomtryPath.c_str());

            _loaded[path] = Varlet::RendererAPI::CreateShader(vertSource, fragSource, geomSource);
        }

        return _loaded[path];
    }

    uint32_t Shader::GetId() const
    {
        return _id;
    }

    std::string Shader::Load(const char* path)
    {
        std::ifstream stream;
        stream.open(path);

        if (stream.is_open())
        {
            std::stringstream buffer;
            buffer << stream.rdbuf();
            return buffer.str();
        }
        else
        {
            VARLET_LOG(LevelType::Warning, "Failed load shader: " + std::string(path));
            return std::string();
        }
    }
}
