#pragma once

#include "EditorCoreCore.h"
#include "EditorPanel.generated.h"

/*
 * Base class for rendered editor panel
 */
REFLECTABLE(Abstract)
class EDITORCORE_API EditorPanel
{
	GENERATION

private:

	PanelUtility* _utility;

public:

	virtual ~EditorPanel() = default;

	virtual void Initialize();

	virtual void Shutdown();

	void Initialize(PanelUtility* utility);
};
