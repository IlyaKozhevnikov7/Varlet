#include "OpenGL/RenderStages/DebugRenderStage.h"
#include "OpenGL/DescriptorPool.h"
#include "OpenGL/Camera.h"
#include "Core/Debug.h"
#include <glad/glad.h>

#define UPDATE_BUFFER(UpdatableType, Buffer, Elements, ...) \
	bool shouldUpdateAgain = false; \
	Buffer.verticesCount = 0; \
	std::vector<UpdatableType> updatable; \
	for (const auto& element : Elements) \
		if (element.duration > 0.f || element.duration == -1.f) \
		{ \
			updatable.push_back({ __VA_ARGS__ }); \
			++Buffer.verticesCount; \
			if (Buffer.verticesCount > geometryAmountInBuffer) \
			{ \
				shouldUpdateAgain = true; \
				break; \
			} \
		} \
	glBindBuffer(GL_ARRAY_BUFFER, Buffer.vbo); \
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(UpdatableType) * updatable.size(), updatable.data()); \
	return shouldUpdateAgain; \

#define FIELD(Name) element.Name

#define DRAW_GEOMETRY(Shader, Buffer, UpdateFunc) \
	glUseProgram(Shader); \
	glBindVertexArray(Buffer.vao); \
	do \
	{ \
		shouldUpdateAgain = UpdateFunc(); \
		if (Buffer.verticesCount > 0) \
			glDrawArrays(GL_POINTS, 0, Buffer.verticesCount); \
	} while (shouldUpdateAgain); \

namespace
{
	const char* vertexLineShaderCode =
		R"(
			#version 460

			layout(location = 0) in vec3 aStart;
			layout(location = 1) in vec3 aEnd;
			layout(location = 2) in vec3 aColor;

			out gl_PerVertex
			{
				vec4 gl_Position;
				float gl_PointSize;
				float gl_ClipDistance[];
			};			

			out VS_OUT 
			{
				vec3 start;
				vec3 end;
			    vec3 color;
			} vs_out;

			void main()
			{
				vs_out.start = aStart;
				vs_out.end = aEnd;
				vs_out.color = aColor;
			}
		)";

	const char* geometrLineShaderCode =
		R"(
			#version 460
			
			layout (points) in;
			layout (line_strip, max_vertices = 2) out;

			out gl_PerVertex
			{
				vec4 gl_Position;
				float gl_PointSize;
				float gl_ClipDistance[];
			};

			layout(std140, binding = 0) uniform Camera
			{
			    mat4 u_Projection;
			    mat4 u_View;
			    mat4 u_ProjectionView;
			    mat4 u_Model;
			    vec3 u_CameraPosition;
			};

			in VS_OUT 
			{
				vec3 start;
				vec3 end;
			    vec3 color;
			} gs_in[];

			out GS_OUT 
			{
			    vec3 color;
			} gs_out;

			void main()
			{
				gs_out.color = gs_in[0].color;

				gl_Position = u_ProjectionView * vec4(gs_in[0].start, 1.f);
				EmitVertex();

				gl_Position = u_ProjectionView * vec4(gs_in[0].end, 1.f);
				EmitVertex();

				EndPrimitive();
			}
		)";

	const char* vertexCircleShaderCode =
		R"(
			#version 460

			layout(location = 0) in vec3 aColor;
			layout(location = 1) in float aRadius;
			layout(location = 2) in float aSegmentsAmount;
			layout(location = 3) in mat4 aOrientation;

			out gl_PerVertex
			{
				vec4 gl_Position;
				float gl_PointSize;
				float gl_ClipDistance[];
			};			

			out VS_OUT 
			{
				vec3 color;
				float radius;
				float segmentsAmount;
				mat4 orientation;
			} vs_out;

			void main()
			{
				vs_out.color = aColor;
				vs_out.radius = aRadius;
				vs_out.segmentsAmount = aSegmentsAmount;
				vs_out.orientation = aOrientation;
			}
		)";

	const char* geometrCircleShaderCode =
		R"(
			#version 460
			
			layout (points) in;
			layout (line_strip, max_vertices = 55) out;

			out gl_PerVertex
			{
				vec4 gl_Position;
				float gl_PointSize;
				float gl_ClipDistance[];
			};

			layout(std140, binding = 0) uniform Camera
			{
			    mat4 u_Projection;
			    mat4 u_View;
			    mat4 u_ProjectionView;
			    mat4 u_Model;
			    vec3 u_CameraPosition;
			};

			in VS_OUT 
			{
				vec3 color;
				float radius;
				float segmentsAmount;
				mat4 orientation;
			} gs_in[];

			out GS_OUT 
			{
			    vec3 color;
			} gs_out;

			#define PI 3.1415926535897932384626433832795f

			void main()
			{
				gs_out.color = gs_in[0].color;
				const float a = PI * 2.f / gs_in[0].segmentsAmount;

				for	(int i = -1; i < gs_in[0].segmentsAmount; i++)
				{
					const float x = sin(a * i) * gs_in[0].radius;
					const float y = cos(a * i) * gs_in[0].radius;

					gl_Position = u_ProjectionView * gs_in[0].orientation * vec4(x, y, 0.f, 1.f);
					EmitVertex();
				}

				EndPrimitive();
			}
		)";

	const char* fragmentShaderCode =
		R"(
			#version 460
			
			in GS_OUT 
			{
			    vec3 color;
			} fs_in;
			
			out vec4 fragColor;
			
			void main()
			{
				fragColor = vec4(fs_in.color, 1.f);
			}
		)";
}

namespace Varlet::OpenGL
{
	uint32_t DebugRenderStage::lineShader;
	DebugGeomtryBuffer DebugRenderStage::lineBuffer;

	uint32_t DebugRenderStage::circleShader;
	DebugGeomtryBuffer DebugRenderStage::circleBuffer;

	void DebugRenderStage::Init()
	{
		glLineWidth(1.5f);

		lineShader = CreateShader(vertexLineShaderCode, geometrLineShaderCode, fragmentShaderCode);
		circleShader = CreateShader(vertexCircleShaderCode, geometrCircleShaderCode, fragmentShaderCode);
		
		InitLineBuffer();
		InitCircleBuffer();
	}

	void DebugRenderStage::Shutdown()
	{
		glDeleteProgram(lineShader);
		glDeleteProgram(circleShader);
		
		glDeleteVertexArrays(1, &lineBuffer.vao);
		glDeleteBuffers(1, &lineBuffer.vbo);
		
		glDeleteVertexArrays(1, &circleBuffer.vao);
		glDeleteBuffers(1, &circleBuffer.vbo);
	}

	void DebugRenderStage::Update()
	{
		for (auto [cameraComponent, camera] : DescriptorPool::GetCameras())
		{
			camera->framebuffer.Bind();

			bool shouldUpdateAgain = false;

			DRAW_GEOMETRY(lineShader, lineBuffer, UpdateLineBuffer);
			DRAW_GEOMETRY(circleShader, circleBuffer, UpdateCircleBuffer);

			camera->framebuffer.UnBind();
		}

		glUseProgram(0);
	}

	bool DebugRenderStage::UpdateLineBuffer()
	{
		UPDATE_BUFFER(Line, lineBuffer, DebugData::GetLines(), FIELD(start), FIELD(end), FIELD(color));
	}

	bool DebugRenderStage::UpdateCircleBuffer()
	{
		UPDATE_BUFFER(Cirlce, circleBuffer, DebugData::GetCircles(), FIELD(color), FIELD(radius), static_cast<float>(FIELD(segmentsAmount)), FIELD(model));
	}

	void DebugRenderStage::InitLineBuffer()
	{
		glGenVertexArrays(1, &lineBuffer.vao);
		glGenBuffers(1, &lineBuffer.vbo);
		
		glBindVertexArray(lineBuffer.vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, lineBuffer.vbo);
		glBufferData(GL_ARRAY_BUFFER, lineBufferSize, nullptr, GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Line), reinterpret_cast<void*>(0));
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Line), reinterpret_cast<void*>(sizeof(glm::vec3)));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Line), reinterpret_cast<void*>(sizeof(glm::vec3) * 2));
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		lineBuffer.verticesCount = 0;
	}

	void DebugRenderStage::InitCircleBuffer()
	{
		glGenVertexArrays(1, &circleBuffer.vao);
		glGenBuffers(1, &circleBuffer.vbo);
		
		glBindVertexArray(circleBuffer.vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, circleBuffer.vbo);
		glBufferData(GL_ARRAY_BUFFER, circleBufferSize, nullptr, GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(0));
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(sizeof(glm::vec3)));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(float)));
		
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(float) * 2));
		
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(float) * 2 + sizeof(glm::vec4)));
		
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(float) * 2 + sizeof(glm::vec4) * 2));
		
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Cirlce), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(float) * 2 + sizeof(glm::vec4) * 3));
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		circleBuffer.verticesCount = 0;
	}

	uint32_t DebugRenderStage::CreateShader(const char* vert, const char* geom, const char* frag)
	{
		const uint32_t id = glCreateProgram();
		
		const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const uint32_t geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(vertexShader, 1, &vert, nullptr);
		glShaderSource(geometryShader, 1, &geom, nullptr);
		glShaderSource(fragmentShader, 1, &frag, nullptr);
		
		glCompileShader(vertexShader);
		glCompileShader(geometryShader);
		glCompileShader(fragmentShader);
		
		glAttachShader(id, vertexShader);
		glAttachShader(id, geometryShader);
		glAttachShader(id, fragmentShader);
		
		glLinkProgram(id);
		
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
		
		return id;
	}
}
