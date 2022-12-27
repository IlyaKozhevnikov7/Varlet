#pragma once

#include "OpenGL/RenderStages/IRenderStage.h"
#include "GLM/glm.hpp"
#include <stdint.h>

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

	class DebugRenderStage : public IRenderStage
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

		~DebugRenderStage() override = default;

		void Init() override;

		void Shutdown() override;

		void Update() override;

	private:

		bool UpdateLineBuffer();

		bool UpdateCircleBuffer();

		void InitLineBuffer();

		void InitCircleBuffer();

		uint32_t CreateShader(const char* vert, const char* geom, const char* frag);
	};
}
