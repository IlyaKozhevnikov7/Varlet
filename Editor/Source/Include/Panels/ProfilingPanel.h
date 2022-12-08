#pragma once

#include "Panel.h"

class ProfilingPanel final : public Panel
{
public:

	~ProfilingPanel() = default;

	void Update() override;
};

