#pragma once

#include "Editor.h"
#include "Core/VarletAPI.h"

#include "../../Dependencies/ImGui/imgui.h"
#include "../../Dependencies/ImGui/imgui_internal.h"

#include "../../Dependencies/ImGui/imgui_impl_glfw.h"
#include "../../Dependencies/ImGui/imgui_impl_opengl3.h"

#include "../../Dependencies/ImGui/ImGuizmo.h"

#define NATIVE_TEXTURE_TO_IMTEXTUREID(Texture) ((ImTextureID)NATIVE_TEXTURE_TO_SIZE_T(Texture))
#define NATIVE_TEXTURE_TO_SIZE_T(Texture) (*(size_t*)(Texture))

#define TEXTURE_FILE_PAYLOAD "TEXTURE_FILE_PAYLOAD"