#include "Utilities/ProjectReloaderUtility.h"
#include "EditorData.h"
#include "Module.h"
#include "ModuleService.h"
#include "CommandLine.h"
#include "Scene.h"
#include "Entity.h"
#include "Components/Component.h"
#include <filesystem>
#include <fstream>

#include <atomic>

std::unordered_map<uint32_t, LastTypeInfo> lastTypeInfos;
std::unordered_map<uint32_t, std::vector<ComponentInfo>> lastComponentInfos;

void ProjectReloaderUtility::Initialize()
{	
	_thread = std::thread([this]()
		{
			const std::wstring runtimeReloaedModulePath = EditorData::ProjectPath + WIDE("\\Cache\\Bin\\Development\\HotReloaded");
			const std::wstring sourceModulePath = EditorData::ProjectPath + WIDE("\\Cache\\Bin\\Development");
			const std::wstring libPath = runtimeReloaedModulePath + WIDE("\\") + EditorData::ProjectName + WIDE(".dll");
			
			PrepareRuntimeFiles(runtimeReloaedModulePath.c_str(), sourceModulePath.c_str());
			LoadTargetModule(EditorData::ProjectPath.c_str());
			
			auto lastReloadTime = std::filesystem::last_write_time(libPath);

			while (true)
			{
				try
				{
					if (auto time = std::filesystem::last_write_time(libPath); time > lastReloadTime)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(250));
			
						EditorData::IsReloading = true;
						Scene::SetShouldUpdate(false);
			
						OnPreReloading();
			
						Varlet::Core::ModuleService::FreeModule(_platformModule);
						EditorData::Module = nullptr;
			
						PrepareRuntimeFiles(runtimeReloaedModulePath.c_str(), sourceModulePath.c_str());
						LoadTargetModule(EditorData::ProjectPath.c_str());
			
						OnPostReloading();
						Scene::SetShouldUpdate(true);
			
						lastReloadTime = time;
					}
				}
				catch (const std::exception& e)
				{
					printf(e.what());
				}
			
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
	});
}

void ProjectReloaderUtility::Shutdown()
{
	_thread.detach();
}

void ProjectReloaderUtility::LoadTargetModule(const wchar_t* path)
{
	EditorData::Module = Varlet::Core::ModuleService::LoadModule(path, false);
	_platformModule = Varlet::Core::ModuleService::GetPlatformModule(EditorData::Module);

	VARLET_ASSERT(EditorData::Module != nullptr, "Editor project module is null");

	EditorData::Module->Initialize();
	EditorData::Module->PostInitialize();

	ReceiveAllAvailableComponents();

	EditorData::IsReloading = false;
}

void ProjectReloaderUtility::PrepareRuntimeFiles(const wchar_t* fromDirectory, const wchar_t* toDirectory)
{
	for (auto from : std::filesystem::directory_iterator(fromDirectory))
	{
		if (from.is_directory())
			continue;

		const auto extension = from.path().extension().wstring();

		if (extension == WIDE(".pdb"))
			continue;

		std::wstring to = toDirectory;
		to += WIDE("\\") + EditorData::ProjectName + extension;
		
		std::filesystem::copy_file(from, to, std::filesystem::copy_options::overwrite_existing);
	}
}

void ProjectReloaderUtility::ReceiveAllAvailableComponents()
{
	EditorData::ContextComponentTypes = Varlet::Core::Type::FindAllTypesByPredicate([](const Varlet::Core::Type* type)
		{
			return type->IsA(Component) && type->flags.NoHas(Varlet::Core::TypeMetaFlags::Abstract);
		});
}

void ProjectReloaderUtility::OnPreReloading()
{
	const auto& entities = Scene::GetAllEntities();

	for (auto entity : entities)
	{
		const auto& components = entity->GetComponents();

		int32_t index = 0;

		while (index < components.size())
		{
			const auto currentComponent = components[index];
			const auto type = currentComponent->GetType();

			if (type->module != EditorData::Module)
			{
				++index;
				continue;
			}

			TrySaveLastType(type);
			SaveComponentInfo(currentComponent);

			entity->RemoveComponent(currentComponent);
		}
	}
}

void ProjectReloaderUtility::OnPostReloading()
{
	for (auto& lastComponentInfo : lastComponentInfos)
	{
		const auto& componentInfos = lastComponentInfo.second;

		const auto componentType = Varlet::Core::Type::GetType(lastComponentInfo.first);
		const auto& lastComponentType = lastTypeInfos[lastComponentInfo.first];

		for (auto& info : componentInfos)
		{
			if (componentType != nullptr)
			{
				const auto newComponent = info.owner->AddComponent(componentType);
				RestoreObject((int8_t*)newComponent, componentType, (int8_t*)info.data, lastComponentType);
			}

			// TODO: могут остаться висячие ссылки после удаления
			memset(info.data, 0, lastComponentType.size);
			free(info.data);
		}
	}

	lastTypeInfos.clear();
	lastComponentInfos.clear();
}

void ProjectReloaderUtility::SaveComponentInfo(Component* component)
{
	const auto type = component->GetType();

	ComponentInfo info =
	{
		malloc(type->size),
		component->GetOwner()
	};

	memcpy_s(info.data, type->size, component, type->size);

	lastComponentInfos[type->id].push_back(info);
}

bool ProjectReloaderUtility::TrySaveObjectInfo(int8_t* object, const Varlet::Core::Type* type)
{
	assert(object);
	assert(type);

	// TODO: process template types
	if (type->module != EditorData::Module || lastTypeInfos.contains(type->id))
		return false;
	
	lastTypeInfos[type->id] = { type->size };
	
	for (auto& fieldInfo : type->fieldInfos)
	{
		if (fieldInfo.flags.Has(Varlet::Core::Static))
			continue;

		const auto fieldType = Varlet::Core::Type::GetType(fieldInfo.typeId);
		TrySaveObjectInfo(object + fieldInfo.offset, fieldType);

		lastTypeInfos[type->id].fieldInfos.push_back(fieldInfo);
	}
}

void ProjectReloaderUtility::TrySaveLastType(const Varlet::Core::Type* type)
{
	if (type->module != EditorData::Module || lastTypeInfos.contains(type->id))
		return;

	lastTypeInfos[type->id] = { type->size };

	for (auto& fieldInfo : type->fieldInfos)
	{
		if (fieldInfo.flags.Has(Varlet::Core::Static))
			continue;

		const auto fieldType = Varlet::Core::Type::GetType(fieldInfo.typeId);

		if (fieldType == nullptr)
			continue;

		TrySaveLastType(fieldType);

		lastTypeInfos[type->id].fieldInfos.push_back(fieldInfo);
	}
}

void ProjectReloaderUtility::RestoreObject(int8_t* newObject, const Varlet::Core::Type* type, int8_t* oldObject, const LastTypeInfo& lastType)
{
	assert(newObject);
	assert(type);
	assert(oldObject);

	for (auto& fieldInfo : type->fieldInfos)
	{
		const auto fieldType = Varlet::Core::Type::GetType(fieldInfo.typeId);

		const auto it = std::find_if(lastType.fieldInfos.begin(), lastType.fieldInfos.end(), [&fieldInfo](const Varlet::Core::FieldInfo& info)
			{
				return info.name == fieldInfo.name
					&& info.typeId == fieldInfo.typeId
					&& info.flags == fieldInfo.flags;
			});

		if (it != lastType.fieldInfos.end())
		{
			if (lastTypeInfos.contains(fieldType->id))
			{
				RestoreObject(newObject + fieldInfo.offset, fieldType, oldObject + (*it).offset, lastTypeInfos[fieldType->id]);
			}
			else
			{
				void* destination = newObject + fieldInfo.offset;
				void* source = oldObject + (*it).offset;
				size_t size = fieldInfo.flags.NoHas(Varlet::Core::Pointer)
					? fieldType->size
					: sizeof(size_t);

				memcpy_s(destination,
					size,
					source,
					size);
			}
		}
	}
}