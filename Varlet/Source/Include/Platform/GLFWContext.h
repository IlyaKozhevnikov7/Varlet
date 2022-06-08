#pragma once

#include "Context.h"

struct GLFWwindow;

namespace Varlet
{
	class CORE_API GLFWContext final : public Context<GLFWwindow>
	{
	public:
	
		int32_t Init() override;
	
		void Update() override;

		void Shutdown() override;
	
		~GLFWContext() override = default;
	};
}
