#pragma once

#include "VarletCore.h"

namespace Varlet
{
	struct CORE_API ShaderInitializer final
	{
	public:

		std::string vertexPath;
		std::string fragmentPath;
		std::string geomtryPath;
	};

	class Shader
	{
	public:

		Shader() = default;

		virtual ~Shader() = default;

		virtual void Use() = 0;
	};
}
