#pragma once

#include "Core/Debug.h"

namespace Varlet::OpenGL
{
	struct DebugGeomtryBuffer final
	{
		uint32_t vao;
		uint32_t vbo;
		uint32_t verticesCount;
	};

	struct IndexedDebugGeomtryBuffer final
	{
		uint32_t vao;
		uint32_t vbo;
		uint32_t ebo;
		uint32_t verticesCount;
	};

	struct Line final
	{
		glm::vec3 start;
		glm::vec3 end;
		glm::vec3 color;
	};

	struct Cirlce final
	{
		glm::vec3 color;
		float radius;
		float segmentsAmount;
		glm::mat4 orientation;
	};

	struct DebugGeometry final
	{
	public:

		constexpr static int64_t geometryAmountInBuffer = 100;
		constexpr static int64_t lineBufferSize = sizeof(Line) * geometryAmountInBuffer;
		constexpr static int64_t circleBufferSize = sizeof(Cirlce) * geometryAmountInBuffer;

		static uint32_t lineShader;
		static DebugGeomtryBuffer lineBuffer;

		static uint32_t circleShader;
		static DebugGeomtryBuffer circleBuffer;

	public:

		static void Init();

		static void Shutdown();

		// return should update buffer again
		static bool UpdateLineBuffer();

		static bool UpdateCircleBuffer();

	private:

		static void InitLineBuffer();

		static void InitCircleBuffer();

		static uint32_t CreateShader(const char* vert, const char* geom, const char* frag);
	};
}
