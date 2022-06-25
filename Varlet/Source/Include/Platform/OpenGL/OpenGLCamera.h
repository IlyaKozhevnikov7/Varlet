#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLCamera final : public CameraCore
	{
	protected:

		void CulculateView() override;
	};
}
