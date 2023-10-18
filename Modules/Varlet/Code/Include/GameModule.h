#pragma once

#include "VarletCore.h"
#include "Module.h"

namespace Varlet
{
	/*
	 *	Varlet::Module ensures proper management of module resources, if any
	 */
	class VARLET_API Module : public Core::Module
	{
	public:

		virtual bool PostInitialize() override;

	private:

		std::wstring ConvertLibraryPathToAssetDirectoryPath(const std::wstring& pathString);

		void RegisterAllAssets(const std::wstring& from);
	};
}
