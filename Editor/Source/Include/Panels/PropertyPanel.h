#pragma once

#include "Panel.h"

namespace Varlet
{
	struct Property;
}

class PropertyPanel final : public Panel
{
public:

	void Update() override;

private:

	void DisplayProperty(const Varlet::Property& property) const;
};