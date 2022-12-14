#include "Debug.h"
#include "Time/VarletTime.h"
#include "Debug.h"

#define REGISTER_GEOMETRY(Type, Container, ...) \
	Type newObject = { __VA_ARGS__ }; \
	for (auto& element : Container) \
		if (element.duration <= 0.f && element.duration > -1.f) \
		{ \
			element = newObject; \
			return; \
		} \
	Container.push_back(newObject); \

#define UPDATE_DEOMETRY(Container, DeltaTime) \
	for (auto& element : Container) \
		if (element.duration > 0.f) \
			element.duration -= deltaTime; \
		else if (element.duration == -1.f) \
			element.duration = 0.f; \
		

void Debug::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float duration)
{
	REGISTER_GEOMETRY(Varlet::DebugData::Line, Varlet::DebugData::_lines, start, end, color, duration);
}

void Debug::DrawCircle(glm::vec3 center, glm::vec3 rotation, float radius, glm::vec3 color, int32_t segmentsAmount, float duration)
{
	segmentsAmount = glm::clamp(segmentsAmount, 4, segmentsAmount);

	glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(center));
	model *= glm::mat4_cast(glm::quat(glm::radians(rotation)));

	REGISTER_GEOMETRY(Varlet::DebugData::Circle, Varlet::DebugData::_circles, radius, color, segmentsAmount, duration, model);
}

namespace Varlet
{
	std::vector<DebugData::Line> DebugData::_lines;
	std::vector<DebugData::Circle> DebugData::_circles;

	void DebugModule::Update()
	{
		const float deltaTime = Time::GetDeltaTime();

		UPDATE_DEOMETRY(DebugData::_lines, deltaTime);
		UPDATE_DEOMETRY(DebugData::_circles, deltaTime);
	}

	const std::vector<DebugData::Line>& DebugData::GetLines()
	{
		return _lines;
	}

	const std::vector<DebugData::Circle>& DebugData::GetCircles()
	{
		return _circles;
	}
}
