#pragma once

#include "VarletCore.h"
#include "Module.h"

class CORE_API Debug final
{
public:
	
	// -1.f duration mean one frame
	static void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float duration = -1.f);

	static void DrawCircle(glm::vec3 center, glm::vec3 rotation, float radius, glm::vec3 color, int32_t segmentsAmount, float duration = -1.f);
};

namespace Varlet
{
	class DebugModule final : public Module, public IUpdatebleModule
	{
	public:

		~DebugModule() override = default;

		void Update() override;
	};

	class CORE_API DebugData final
	{
		friend Debug;
		friend DebugModule;

	public:

		struct Line final
		{
			glm::vec3 start;
			glm::vec3 end;
			glm::vec3 color;
			float duration;
		};

		struct Circle final
		{
			float radius;
			glm::vec3 color;
			float segmentsAmount; // TODO float because opengl does not support integer attributes
			float duration;
			glm::mat4 model;
		};

	private:

		static std::vector<Line> _lines;
		static std::vector<Circle> _circles;

	public:

		// TODO make iterator for only active geometry
		static const std::vector<Line>& GetLines();

		static const std::vector<Circle>& GetCircles();
	};
}
