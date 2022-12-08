#pragma once

#include <vector>

namespace Varlet::OpenGL
{
	struct Framebuffer final
	{
	public:

		int32_t width;
		int32_t height;

		uint32_t id;
		uint32_t renderbufferId; // Depth and stencil buffer

		std::vector<uint32_t> attachments; // [0] attachment color

	public:

		Framebuffer();

		Framebuffer(const int32_t& width, const int32_t& height);

		void Bind() const;

		void UnBind() const;

		void Resize(const int32_t& width, const int32_t& height);

		std::vector<uint8_t> ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment);

		static void Free(Framebuffer& framebuffer);

	private:

		void Initialize(int32_t width, int32_t height, int32_t attachmentCount);
	};

	struct Camera final
	{
	public:
		
		Framebuffer framebuffer;

	public:

		Camera(int32_t width, int32_t height);
		
		~Camera();

		void Resize(int32_t width, int32_t height, const bool& dynamically);

		void Bind() const;

		std::vector<uint8_t> ReadPixels(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& attachment);
	};
}
