#pragma once

#include "Panel.h"

class ProfilingPanle final : public Panel
{
public:

	~ProfilingPanle() = default;

	void Update() override;
};

