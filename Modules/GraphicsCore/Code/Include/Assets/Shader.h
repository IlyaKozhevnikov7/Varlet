#pragma once

#include "GraphicsCoreCore.h"
#include "Asset.h"
#include "Shader.generated.h"

struct UniformInfo
{
	std::string name;
	int32_t size;
	int32_t offset;
	const Varlet::Core::Type* type;
};

REFLECTABLE()
class GRAPHICSCORE_API Shader final : public IAsset, public ICustomSerializable
{
	GENERATION

	struct ShaderStages final
	{
		std::string vertexStage;
		std::string fragmentStage;
		std::string geometryStage;

		bool IsCompleted() const;
	};

	struct StageDefinition final
	{
		std::string stageName;
		int32_t position;
	};

private:

	static std::unordered_map<std::wstring, Shader*> _loaded;

private:

	Shader() = default;

public:

	static Shader* Create(const wchar_t* path, bool compileAsSource = false);

	~Shader() override = default;

	void Serialize(ISerializeContext* context) override;

	bool Deserialize(IDeserializeContext* context) override;

private:

	static bool CreateInternal(const wchar_t* path, Shader* shader, bool compileAsSource);

	static std::string Load(const wchar_t* path);
};