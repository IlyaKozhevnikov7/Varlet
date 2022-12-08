#pragma once

#include <unordered_map>

class Camera;
class Mesh;

namespace Varlet 
{
	class Shader;
	class Texture;
	struct MeshData;
}

namespace Varlet::OpenGL
{
	struct Camera;
	struct VertexBuffer;

	class Shader;
	class Texture;

	struct DescriptorPool final
	{
		using CameraComponent = ::Camera;

	private:

		static std::unordered_map<CameraComponent*, Camera*> _cameras;
		static std::vector<Shader*> _shaders;
		static std::vector<Texture*> _textures;
		static std::unordered_map<const Mesh*, std::vector<VertexBuffer>> _vertexBuffers;

	public:

		static void Shutdown();

		static const std::unordered_map<CameraComponent*, Camera*>& GetCameras();

		static Camera* GetCamera(CameraComponent* camera);

		static uint32_t* GetNativeTexture(const Varlet::Texture* texture);

		static Shader* GetShader(const Varlet::Shader* shader);

		static const std::vector<VertexBuffer>& GetVertexBufferSet(const Mesh* mesh);

		static void Register(CameraComponent* camera);

		static void Register(Texture* texture);

		static void Register(Shader* shader);

		static void Commit(const Mesh* mesh, const std::vector<MeshData>& vertices);

		static void Unregister(CameraComponent* camera);

		static void Unregister(Varlet::Shader* shader);

		static void Unregister(Varlet::Texture* texture);
	};
}