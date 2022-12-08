#include "OpenGL/DescriptorPool.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include "OpenGL/VertexBuffer.h"

#include "Scene/Camera.h"
#include "Rendering/Mesh.h"

namespace Varlet::OpenGL
{
    std::unordered_map<DescriptorPool::CameraComponent*, Camera*> DescriptorPool::_cameras;
    std::vector<Shader*> DescriptorPool::_shaders;
    std::vector<Texture*> DescriptorPool::_textures;
    std::unordered_map<const Mesh*, std::vector<VertexBuffer>> DescriptorPool::_vertexBuffers;

    void DescriptorPool::Shutdown()
    {
        for (const auto& camera : _cameras)
            delete camera.second;

        for (const auto& shader : _shaders)
            delete shader;

        for (const auto& texture : _textures)
            delete texture;

        for (const auto& bufferStack : _vertexBuffers)
            for (const auto& buffer : bufferStack.second)
                VertexBuffer::Free(buffer);
    }

    const std::unordered_map<DescriptorPool::CameraComponent*, Camera*>& DescriptorPool::GetCameras()
    {
        return _cameras;
    }

    Camera* DescriptorPool::GetCamera(DescriptorPool::CameraComponent* camera)
    {
        return _cameras[camera];
    }

    uint32_t* DescriptorPool::GetNativeTexture(const Varlet::Texture* texture)
    {
        for (auto& nativeTexture : _textures)
            if (texture == nativeTexture)
                return &nativeTexture->id;
        
        return nullptr;
    }

    Shader* DescriptorPool::GetShader(const Varlet::Shader* shader)
    {
        for (const auto& nativeShader : _shaders)
            if (shader == nativeShader)
                return nativeShader;

        return nullptr;
    }

    const std::vector<VertexBuffer>& DescriptorPool::GetVertexBufferSet(const Mesh* mesh)
    {
        return _vertexBuffers[mesh];
    }

    void DescriptorPool::Register(CameraComponent* camera)
    {
        int32_t width, height;
        camera->GetResolution(width, height);
        _cameras[camera] = new Camera(width, height);
    }

    void DescriptorPool::Register(Texture* texture)
    {
        _textures.push_back(texture);
    }

    void DescriptorPool::Register(Shader* shader)
    {
        _shaders.push_back(shader);
    }

    void DescriptorPool::Commit(const Mesh* mesh, const std::vector<MeshData>& vertices)
    {
        if (vertices.size() > 0)
        {
            VARLET_ASSERT(_vertexBuffers.contains(mesh) == false, "The same mesh object cannot be loaded twice");
            
            const int32_t subMeshCount = vertices.size();
            std::vector<VertexBuffer> vertexBufferSet(subMeshCount);

            for (int32_t i = 0; i < subMeshCount; i++)
                vertexBufferSet[i] = VertexBuffer(vertices[i]);

            _vertexBuffers[mesh] = vertexBufferSet;
        }
        else
        {
            const MeshData data =
            {
                mesh->positions,
                mesh->normals,
                mesh->uvs,
                mesh->tangents,
                mesh->indices
            };

            if (_vertexBuffers.contains(mesh) == false)
                _vertexBuffers[mesh] = std::vector<VertexBuffer>(1);
            else
                VertexBuffer::Free(_vertexBuffers[mesh][0]);

            _vertexBuffers[mesh][0] = VertexBuffer(data);
        }
    }

    void DescriptorPool::Unregister(CameraComponent* camera)
    {
        _cameras.erase(camera);
    }

    void DescriptorPool::Unregister(Varlet::Shader* shader)
    {
        for (auto& nativeShader : _shaders)
            if (shader == nativeShader)
                nativeShader = nullptr;
    }

    void DescriptorPool::Unregister(Varlet::Texture* texture)
    {
        for (auto& nativeTexture : _textures)
            if (texture == nativeTexture)
                nativeTexture = nullptr;
    }
}
