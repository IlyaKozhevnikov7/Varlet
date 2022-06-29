#pragma once

#include "VarletFramework.h"

class EditorCamera final : public Component
{
private:

	Transform* _transform;

public:

	void InternalStart();

	void InternalUpdate();
};

