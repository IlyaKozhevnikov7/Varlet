#include "Assets/Texture2D.h"
#include "TextureLoader.h"
#include "AssetPath.h"
#include "../../../GraphicsInterface/Code/Include/GraphicsAPI.h"

std::unordered_map<std::wstring, Texture2D*> Texture2D::_loaded;

Texture2D* Texture2D::Load(const wchar_t* path, bool mipmap, bool flipUV, WrapType wrapType, FilterType filter)
{
	const auto newTextue = new Texture2D(wrapType, filter);

	if (CreateInternal(path, flipUV, wrapType, filter, newTextue))
	{
		_loaded[path] = newTextue;
	}
	else
	{
		delete newTextue;
		return nullptr;
	}

	return _loaded[path];
}

Texture2D::Texture2D(WrapType wrapType, FilterType filter) :
	_wrapType(wrapType),
	_filter(filter)
{
}

void Texture2D::Serialize(ISerializeContext* context)
{
	context->AddData(_wrapType);
	context->AddData(_filter);

	for (auto& entry : _loaded)
		if (entry.second == this)
		{
			auto serializedPath = AssetPath::SerializePath(entry.first);
			context->AddData(serializedPath);
			return;
		}
}

bool Texture2D::Deserialize(IDeserializeContext* context)
{
	_wrapType = context->Read<WrapType>();
	_filter = context->Read<FilterType>();

	const size_t pathSize = context->GetDataSize() - sizeof(WrapType) - sizeof(FilterType);
	int8_t* dataPtr = (int8_t*)context->Read(pathSize);
	auto data = std::vector<int8_t>(dataPtr, dataPtr + pathSize);

	std::wstring path = AssetPath::DeserializePath(data);

	const bool isSuccess = CreateInternal(path.c_str(), true, _wrapType, _filter, this);

	if (isSuccess)
		_loaded[path] = this;

	return isSuccess;
}

bool Texture2D::CreateInternal(const wchar_t* path, bool flipUV, WrapType wrapType, FilterType filter, Texture2D* texture)
{
	int32_t width, height;
	TextureFormat format;

	void* data = Varlet::TextureLoader::Load(path, flipUV, format, width, height);

	if (data == nullptr)
	{
		VARLET_LOG(Warning, WIDE("Texture \"%s\" load error"), path);
		return false;
	}

	Varlet::Graphics::TextureCreateInfo createInfo;
	createInfo.width = width;
	createInfo.height = height;
	createInfo.data = data;
	createInfo.type = TextureType::Texture2D;
	createInfo.format = format;
	createInfo.memoryProperty = Varlet::Graphics::MemoryProperty::Device;
	createInfo.generateMipmap = true;
	createInfo.wrapType = wrapType;
	createInfo.filterType = filter;

	if (Varlet::Graphics::CreateTexture(texture, createInfo) == false)
		return false;

	VARLET_LOG(Normal, WIDE("Texture2D \"%s\" has been loaded"), path);
	Varlet::TextureLoader::Free(data);

	return true;
}
