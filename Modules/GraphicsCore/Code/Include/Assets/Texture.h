#pragma once

#include "GraphicsCoreCore.h"
#include "Asset.h"
#include "GraphicsBaseTypes.h"
#include "Texture.generated.h"

REFLECTABLE(Abstract)
class GRAPHICSCORE_API Texture : public IAsset
{
	GENERATION

protected:

	FIELD()
	int32_t _width = 0;

	FIELD()
	int32_t _height = 0;

	FIELD()
	TextureFormat _format = TextureFormat::RGB;

protected:

	Texture() = default;

public:

	virtual ~Texture() = default;

	int32_t GetWidth() const;

	int32_t GetHeight() const;

	TextureFormat GetFormat() const;
};
