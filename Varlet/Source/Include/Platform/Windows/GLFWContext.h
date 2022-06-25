#pragma once

#include "VarletCore.h"
#include "Context.h"

struct GLFWwindow;

namespace Varlet
{
	class GLFWContext final : public Context<GLFWwindow>
	{
	public:
	
		int32_t Init() override;
	
		bool IsRunning() const override;

		void Update() override;

		void Shutdown() override;
	
		~GLFWContext() override = default;
	};
}
