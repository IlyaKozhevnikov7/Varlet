#include "OpenGL/Command.h"
#include "OpenGL/DebugGeometry.h"

#include <glad/glad.h>

#define DRAW_GEOMETRY(Shader, Buffer, UpdateFunc) \
	glUseProgram(DebugGeometry::Shader); \
	glBindVertexArray(DebugGeometry::Buffer.vao); \
	do \
	{ \
		shouldUpdateAgain = DebugGeometry::UpdateFunc(); \
		if (DebugGeometry::Buffer.verticesCount > 0) \
			glDrawArrays(GL_POINTS, 0, DebugGeometry::Buffer.verticesCount); \
	} while (shouldUpdateAgain); \

namespace Varlet::OpenGL
{
	void Command::DrawDebugGeomtry()
	{
		bool shouldUpdateAgain = false;

		DRAW_GEOMETRY(lineShader, lineBuffer, UpdateLineBuffer);
		DRAW_GEOMETRY(circleShader, circleBuffer, UpdateCircleBuffer);

		glUseProgram(0);
	}
}
