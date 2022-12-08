#pragma once

#include "Rendering/RendererAPI.h"

namespace Varlet::OpenGL
{
	class RendererAPI final : public IRendererAPI
	{
	public:
		
		~RendererAPI() override = default;

		Varlet::Shader* CreateShader(const std::string& vertSource, const std::string& fragSource, const std::string& geomSource) const override;

		Varlet::Texture* CreateTexture(const LoadableTextureConfiguration& configuration) const override;

		void RegisterMesh(const Mesh* mesh, const std::vector<MeshData>& vertices) const override;

		Varlet::Texture* GetTextureOf(::Camera* camera) const override;

		void ChangeResolution(::Camera* camera) const override;

		void GetFramebufferSize(::Camera* camera, int32_t& width, int32_t& height) const override;

		std::vector<uint8_t> ReadRenderTexturePixels(::Camera* camera, const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment = 0) const override;

		const void* GetNativeRenderTexture(::Camera* camera, const uint32_t& attachment = 0) const override;

		const void* GetNativeTexture(Varlet::Texture* texture) const override;

		void DestroyCamera(::Camera* camera) const override;
	};
}
