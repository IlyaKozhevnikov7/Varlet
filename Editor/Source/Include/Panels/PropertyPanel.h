#pragma once

#include "Panel.h"

namespace Varlet
{
	struct Property;
	struct Array;
}

class Object;

class PropertyPanel final : public Panel
{
public:

	void Update() override;

private:

	void DisplayProperty(const Varlet::Property* property) const;

	void DisplayObject(Object** object, const int32_t& id = -1) const;

	void DisplayArray(const Varlet::Array* property) const;

	void DisplaySampler2DProperty(const Varlet::Property* property) const;
};