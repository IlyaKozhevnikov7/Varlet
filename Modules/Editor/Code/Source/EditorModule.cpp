#include "EditorModule.h"
#include "EditorData.h"
#include "ProjectAssetRegister.h"
#include "CommandLine.h"

bool EditorModule::Initialize()
{
	std::wstring projectVModulePath = Varlet::Core::CommandLine::Get(1);
	EditorData::ProjectPath = std::filesystem::path(projectVModulePath).parent_path();
	EditorData::ProjectName = std::filesystem::path(EditorData::ProjectPath).filename();
	return true;
}

bool EditorModule::PostInitialize()
{
	ProjectAssetRegister::Initialize();

	return true;
}

void EditorModule::Update()
{
}

void EditorModule::Shutdown()
{
	ProjectAssetRegister::Shutdown();
}