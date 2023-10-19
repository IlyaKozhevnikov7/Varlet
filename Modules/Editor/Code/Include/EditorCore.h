#pragma once

#ifdef EDITOR_DLL_EXPORT
	#define EDITOR_API __declspec(dllexport)
#else
	#define EDITOR_API __declspec(dllimport)
#endif

#include "EditorCoreCore.h"
#include "VarletCore.h"
#include "EditorCommonInclude.generated.h"

#define NATIVE_TEXTURE_TO_IMTEXTUREID(Texture) ((ImTextureID)NATIVE_TEXTURE_TO_SIZE_T(Texture))
#define NATIVE_TEXTURE_TO_SIZE_T(Texture) (*(size_t*)((void*)Texture))

#define ENTITY_PAYLOAD "ENTITY_PAYLOAD"
