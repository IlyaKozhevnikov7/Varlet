#pragma once

#include "Texture.h"
#include "Texture2D.generated.h"

REFLECTABLE()
class GRAPHICSCORE_API Texture2D final : public Texture, public ICustomSerializable
{
	GENERATION

private:

	static std::unordered_map<std::wstring, Texture2D*> _loaded;

protected:

	WrapType _wrapType;
	FilterType _filter;

private:

	Texture2D() = default;

	Texture2D(WrapType wrapType, FilterType filter);

public:

	~Texture2D() override = default;

	static Texture2D* Load(const wchar_t* path, bool mipmap = true, bool flipUV = true, WrapType wrapType = WrapType::Repeat, FilterType filter = FilterType::Linear);

	void Serialize(ISerializeContext* context);

	bool Deserialize(IDeserializeContext* context);

private:

	static bool CreateInternal(const wchar_t* path, bool flipUV, WrapType wrapType, FilterType filter, Texture2D* texture);
};
