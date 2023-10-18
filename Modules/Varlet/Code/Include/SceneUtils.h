#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class VARLET_API SceneUtils final
	{
	public:

		static void Save(const wchar_t* path);

		static void Load(const wchar_t* path);
	};
}