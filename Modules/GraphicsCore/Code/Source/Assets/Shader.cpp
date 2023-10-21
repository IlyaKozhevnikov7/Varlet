#include "Assets/Shader.h"
#include "AssetPath.h"
#include "GraphicsAPI.h"
#include "GraphicsBaseTypes.h"
#include "Serialization/BinaryFormatter.h"

#include <fstream>

bool Shader::ShaderStages::IsCompleted() const
{
	return vertexStage.empty() == false &&
		(fragmentStage.empty() == false || geometryStage.empty() == false);
}

std::unordered_map<std::wstring, Shader*> Shader::_loaded;

Shader* Shader::Create(const wchar_t* path, bool compileAsSource)
{
	if (_loaded.contains(path) == false)
	{
		const auto shader = new Shader;

		if (CreateInternal(path, shader, compileAsSource))
		{
			_loaded[path] = shader;
		}
		else
		{
			delete shader;
			return nullptr;
		}
	}

	return _loaded[path];
}

void Shader::Serialize(ISerializeContext* context)
{
	for (auto entry : _loaded)
		if (entry.second == this)
		{
			auto serializedPath = AssetPath::SerializePath(entry.first);
			context->AddData(serializedPath);
			return;
		}
}

bool Shader::Deserialize(IDeserializeContext* context)
{
	auto sharedId = context->ReadAll();
	std::wstring path = AssetPath::DeserializePath(sharedId);

	const bool isSuccess = CreateInternal(path.c_str(), this, false);

	if (isSuccess)
		_loaded[path] = this;

	return isSuccess;
}

bool Shader::CreateInternal(const wchar_t* path, Shader* shader, bool compileAsSource)
{
	assert(shader);

	const std::string source = Load(path);

	if (source.empty())
	{
		VARLET_LOG(Error, WIDE("Failed load shader: %s"), path);
		return false;
	}

	Varlet::Graphics::ShaderCreateInfo createInfo;
	createInfo.source = &source;

	return Varlet::Graphics::API::CreateShader(shader, source, compileAsSource);
}

std::string Shader::Load(const wchar_t* path)
{
	if (*path == WIDE('\0'))
		return "";

	std::ifstream stream;
	stream.open(path);

	std::string source;

	if (stream.is_open())
	{
		std::stringstream buffer;
		buffer << stream.rdbuf();
		source = buffer.str();
		stream.close();

		return source;
	}

	return "";
}
