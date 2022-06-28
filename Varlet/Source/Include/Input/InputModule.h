#pragma once

#include "VarletCore.h"
#include "VarletModules.h"

namespace Varlet
{
	class InputModule : public Module
	{
	public:

		virtual int32_t Init() override;

		virtual bool GetKey(const int32_t& keyCode, const int32_t& state) const = 0;

		virtual bool GetMouse(const int32_t& keyCode, const int32_t& state) const = 0;

		virtual float GetMouseX() const = 0;

		virtual float GetMouseY() const = 0;

		virtual glm::vec2 GetMousePosition() const = 0;
	};
}
