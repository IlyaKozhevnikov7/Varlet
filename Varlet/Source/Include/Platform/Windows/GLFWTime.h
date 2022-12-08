#pragma once

#include "VarletCore.h"
#include "VarletTime.h"

namespace Varlet
{
	class CORE_API GLFWTime final : public TimeModule
	{
	private:

		float _now;
		float _lastUpdate;
		float _delta;

	public:

		~GLFWTime() override = default;

		void Update() override;

		virtual float GetTime() override;

		virtual float GetDeltaTime() override;
	};
}
