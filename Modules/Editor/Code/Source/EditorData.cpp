#include "EditorData.h"

std::wstring EditorData::ProjectName;
std::wstring EditorData::ProjectPath;
std::atomic_bool EditorData::IsReloading = false;
Entity* EditorData::SelectedEntity = nullptr;
std::vector<const Varlet::Core::Type*> EditorData::ContextComponentTypes;

Varlet::Core::Module* EditorData::Module = nullptr;