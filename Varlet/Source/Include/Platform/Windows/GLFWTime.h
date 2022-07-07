#pragma once

#include "VarletCore.h"
#include "VarletTime.h"

namespace Varlet
{
	class GLFWTime : public TimeModule
	{
	private:

		float _lastUpdate;
		float _delta;

	public:

		virtual void UpdateTime() override;

		virtual float GetTime() override;

		virtual float GetDeltaTime() override;
	};
}
