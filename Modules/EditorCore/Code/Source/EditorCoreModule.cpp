#include "EditorCoreModule.h"
#include "ModuleService.h"
#include "EditorUtility.h"
#include "Internal/EditorUtilitySystemInternal.h"

bool EditorCoreModule::Initialize()
{
	IModuleAddedRemovedCallback::Initialize();

	Varlet::Core::Platform::CreateWindow("Editor");

	EditorUtilitySystemInternal::Initialize(_utilities);

	return true;
}

void EditorCoreModule::Shutdown()
{
	for (auto utility : _utilities)
		_actions.push_back({ utility, UtilityActionType::Remove });

	ProcessActions();

	IModuleAddedRemovedCallback::Shutdown();
}

void EditorCoreModule::Update()
{
	ProcessActions();

	for (auto utility : _utilities)
	{
		if (auto updatableUtility = Cast<IUpdatableEditorUtility>(utility))
			updatableUtility->Update();
	}
}

void EditorCoreModule::ProcessActions()
{
	if (_actions.size() == 0)
		return;

	for (auto& action : _actions)
	{
		if (action.actionType == UtilityActionType::Add)
		{
			action.utility->Initialize();
			_utilities.push_back(action.utility);
		}
		else
		{
			std::remove(_utilities.begin(), _utilities.end(), action.utility);
			action.utility->Shutdown();
			delete action.utility;
		}
	}

	_actions.clear();
}

void EditorCoreModule::OnRegisteredNewModule(const Varlet::Core::ModuleInfo& info)
{
	auto moduleTypes = Varlet::Core::Type::GetTypes(info.module);

	for (auto type : moduleTypes)
	{
		if (type->flags.NoHas(Varlet::Core::TypeMetaFlags::Abstract) && type->IsA(EditorUtility))
		{
			EditorUtility* newUtility = (EditorUtility*)CastMemory(type, EditorUtility::GetTypeStatic(), std::bit_cast<int8_t*>(type->CreateInstance()));
			assert(newUtility);

			_actions.push_back({ newUtility, UtilityActionType::Add });
		}
	}
}

void EditorCoreModule::OnUnregisteredNewModule(const Varlet::Core::ModuleInfo& info)
{
	if (info.module == this)
		return;

	auto moduleTypes = Varlet::Core::Type::GetTypes(info.module);

	for (auto type : moduleTypes)
	{
		if (type->IsA(EditorUtility))
		{
			auto it = std::find_if(_utilities.begin(), _utilities.end(), [&type](EditorUtility* utility)
				{
					return utility->GetType()->CheckIsA(type);
				});

			if (it != _utilities.end())
				_actions.push_back({ *it, UtilityActionType::Remove });
		}
	}
}
