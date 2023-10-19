#pragma once

#include "EditorCore.h"
#include "EditorUtility.h"
#include "Platform/Current.h"
#include <thread>
#include "ProjectReloaderUtility.generated.h"

struct ComponentInfo final
{
	void* data;
	Entity* owner;
};

struct LastTypeInfo final
{
	size_t size;
	std::vector<Varlet::Core::FieldInfo> fieldInfos;
};

REFLECTABLE()
class ProjectReloaderUtility final : public EditorUtility
{
	GENERATION

private:

	std::thread _thread;
	Varlet::Core::Platform::Module _platformModule;

public:

	~ProjectReloaderUtility() override = default;

	void Initialize() override;

	void Shutdown() override;

private:

	void LoadTargetModule(const wchar_t* path);

	void PrepareRuntimeFiles(const wchar_t* sourceFilesDirectory, const wchar_t* runtimeFilesDirectory);

	void ReceiveAllAvailableComponents();

	void OnPreReloading();

	void OnPostReloading();

	void SaveComponentInfo(Component* component);

	bool TrySaveObjectInfo(int8_t* object, const Varlet::Core::Type* type);

	void TrySaveLastType(const Varlet::Core::Type* type);

	void RestoreObject(int8_t* newObject, const Varlet::Core::Type* type, int8_t* oldObject, const LastTypeInfo& lastType);
};
