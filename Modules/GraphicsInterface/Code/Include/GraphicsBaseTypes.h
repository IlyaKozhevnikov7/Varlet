#pragma once

#include "GraphicsInterfaceCore.h"
#include "GraphicsBaseTypes.generated.h"

enum class TextureFormat : uint8_t
{
	RED = 0,
	RG,
	RGB,
	RGBA,
	RGB111110F,
	RGB16F,
	RGBA16F,
	RGB32F,
	RGBA32F,
	D24S8
};

enum class WrapType : uint8_t
{
	Repeat = 0,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

enum class FilterType : uint8_t
{
	Linear = 0,
	Nearers
};

enum class TextureType : uint8_t
{
	Texture1D,
	Texture2D,
	Cubmap
};

namespace Varlet::Graphics
{
	class RenderPipeline;
}

namespace Varlet::Graphics
{
	/*
	*	Buffers
	*/

	enum class BufferUsage : uint8_t
	{
		Vertex = 0,
		Index,
		Uniform,
		Storage
	};

	enum class MemoryProperty : uint8_t
	{
		Host = 0,
		Device
	};

	struct UniformBufferCreateInfo final
	{
		std::string bindingName;
		uint32_t size;
		BufferUsage usage = BufferUsage::Uniform;
		int8_t* initialData = nullptr; /* Optinal */
	};

	/*
	*	Shader
	*/

	enum class ShaderDataType : uint8_t
	{
		Bool = 0,
		Int,
		UInt,
		Float,
		Double,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4,
		Sampler2D
	};

	struct ShaderCreateInfo final
	{
		const std::string* source;
		void* pipelineType = nullptr;
	};

	struct UniformInfo final
	{
		std::string name;
		ShaderDataType type;
	};

	/*
	*	Texture
	*/

	struct TextureCreateInfo final
	{
		uint32_t width;
		uint32_t height;
		void* data;
		TextureType type;
		TextureFormat format;
		MemoryProperty memoryProperty;
		bool generateMipmap;
		WrapType wrapType;
		FilterType filterType;
	};

	/*
	*	Model
	*/

	struct VertexAttributeInfo final
	{
		ShaderDataType type;
		uint32_t location; // TODO: maybe remove this
	};

	struct SubMeshInfo final
	{
		uint32_t verticesCount;
		std::vector<int8_t> vertexData;

		std::vector<uint32_t> indices;
	};

	struct ModelCreateInfo final
	{
		std::vector<VertexAttributeInfo> vertexLayout;
		std::vector<SubMeshInfo> subMeshInfos;
	};

	/*
	*	 Framebuffer
	*/

	struct PipelinePassInfo final
	{
		/*
		*  Maybe later define attachments refs
		*/
		uint32_t framebufferIndex = 0;
		std::vector<void*> inputAttachments;

	public:

		PipelinePassInfo(uint32_t framebufferIndex, std::initializer_list<void*> inputAttachments) :
			framebufferIndex(framebufferIndex),
			inputAttachments(inputAttachments)
		{
		}

		PipelinePassInfo(const PipelinePassInfo&) = default;

		PipelinePassInfo(PipelinePassInfo&& other) noexcept
		{
			framebufferIndex = other.framebufferIndex;
			inputAttachments = std::move(other.inputAttachments);
		}

		PipelinePassInfo& operator=(PipelinePassInfo&& other) noexcept
		{
			framebufferIndex = other.framebufferIndex;
			inputAttachments = std::move(other.inputAttachments);
		
			return *this;
		}
	};

	struct AttachmentInfo final
	{
		/*
		*	This id use for detect texture in render pass (inputAttachments in SubpassInfo)
		*/
		TextureFormat format = TextureFormat::RGB;
		void* mapped = nullptr; /* Optinal */
	};

	struct RenderTargetCreateInfo final
	{
		uint32_t width;
		uint32_t height;
		TextureFormat format;
		void* descriptor;
		FilterType filter = FilterType::Nearers;
	};

	struct RenderPassInfo final
	{
		std::vector<void*> attachments;
	};

	struct RenderPipelineCreateInfo final
	{
		std::vector<RenderTargetCreateInfo> renderTargetCreateInfos;
		std::vector<RenderPassInfo> renderPasses;

		void Combine(RenderPipelineCreateInfo& other)
		{
			renderTargetCreateInfos.insert(renderTargetCreateInfos.end(), other.renderTargetCreateInfos.begin(), other.renderTargetCreateInfos.end());
			renderPasses.insert(renderPasses.end(), other.renderPasses.begin(), other.renderPasses.end());
		}
	};
}