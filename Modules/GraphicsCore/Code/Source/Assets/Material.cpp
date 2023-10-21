#include "Assets/Material.h"
#include "Assets/Shader.h"
#include "Assets/Texture.h"
#include "AssetPath.h"
#include "GraphicsAPI.h"

// TODO: Fix name conflict
#include "../../../GraphicsInterface/Code/Include/GraphicsAPI.h"

std::unordered_map<Shader*, ShaderUniformLayoutInfo> Material::_uniformLayouts;

Material::Material(Shader* shader)
{
	isActive = true;
	_shader = nullptr;

	SetShader(shader);
}

void Material::SetShader(Shader* newShader)
{
	VARLET_ASSERT(newShader, "Shader is null");

	if (_shader == newShader)
	{
		// TODO: Log
		return;
	}

	_shader = newShader;

	if (_uniformLayouts.contains(_shader) == false)
	{
		const auto nativeUniformInfos = Varlet::Graphics::GetShaderUniformInfos(_shader);
		VARLET_ASSERT(nativeUniformInfos, "Shader uniform infos is null");

		uint32_t bufferSize = 0;
		uint32_t serializedBufferSize = 0;
		auto& newUniformInfos = _uniformLayouts[_shader];

		for (const auto& info : *nativeUniformInfos)
		{
			const uint32_t dataSize = GetShaderTypeSize(info.type);
			const auto type = ShaderDataTypeToType(info.type);

			newUniformInfos.uniformInfos[info.name] = { dataSize, bufferSize, type };
			bufferSize += dataSize;
		}

		newUniformInfos.size = bufferSize;
	}

	const uint32_t bufferSize = _uniformLayouts[_shader].size;
	uniformBuffer.resize(bufferSize);
	uniformBuffer.assign(bufferSize, 0);
}

Shader* Material::GetShader()
{
	return _shader;
}

void Material::SetBool(const char* name, bool value)
{
	SetUniformValue(name, &value);
}

void Material::SetUInt32(const char* name, uint32_t value)
{
	SetUniformValue(name, &value);
}

void Material::SetInt32(const char* name, int32_t value)
{
	SetUniformValue(name, &value);
}

void Material::SetFloat(const char* name, float value)
{
	SetUniformValue(name, &value);
}

void Material::SetVec2(const char* name, const glm::vec2& value)
{
	SetUniformValue(name, &value);
}

void Material::SetVec3(const char* name, const glm::vec3& value)
{
	SetUniformValue(name, &value);
}

void Material::SetVec4(const char* name, const glm::vec4& value)
{
	SetUniformValue(name, &value);
}

void Material::SetMat3(const char* name, const glm::mat3& value)
{
	SetUniformValue(name, &value);
}

void Material::SetMat4(const char* name, const glm::mat4& value)
{
	auto& layout = _uniformLayouts[_shader];

	if (layout.uniformInfos.contains(name))
	{
		const auto& info = layout.uniformInfos[name];
		memcpy(uniformBuffer.data() + info.offset, &value, info.size);
	}
}

void Material::SetTexture2D(const char* name, Texture2D* value)
{
	SetUniformValue(name, value);
}

const ShaderUniformLayoutInfo& Material::GetUniformLayoutInfo() const
{
	return _uniformLayouts[_shader];
}

/*
*	Just for better API
*/
const std::vector<int8_t>& Material::ReceiveUniformBuffer(Material* material)
{
	return material->uniformBuffer;
}

void Material::Serialize(ISerializeContext* context)
{
	context->AddData(isActive);

	auto shaderSharedId = _shader->GetSharedId();
	context->AddData<uint32_t>(shaderSharedId.size());
	context->AddData(shaderSharedId);

	// >> uniform count
	// >> 1 uniform: type id / value
	// >> 2 uniform: type id / value
	// >> 3 uniform: type id / value

	const auto& uniformLayout = _uniformLayouts[_shader];
	std::vector<int8_t> serializeData;
	serializeData.reserve(uniformBuffer.size());

	for (const auto& uniformInfo : uniformLayout.uniformInfos)
	{
		const auto uniformType = uniformInfo.second.type;

		const int8_t* typeIdPtr = std::bit_cast<const int8_t*>(&uniformType->id);
		serializeData.insert(serializeData.end(), typeIdPtr, typeIdPtr + sizeof(uint32_t));
		int8_t* value = &uniformBuffer[0] + uniformInfo.second.offset;

		if (uniformInfo.second.type->IsA(Texture))
		{
			int8_t** textureValue = (int8_t**)value;

			if (*textureValue == nullptr)
			{
				serializeData.insert(serializeData.end(), sizeof(uint32_t), 0);
				continue;
			}

			ISharedSerializable* sharedSerializable = CastMemory<ISharedSerializable>(uniformType, *textureValue);
			assert(sharedSerializable);


			auto sharedId = sharedSerializable->GetSharedId();
			const uint32_t sharedIdSize = sharedId.size();
			const int8_t* sharedIdSizePtr = std::bit_cast<const int8_t*>(&sharedIdSize);

			serializeData.insert(serializeData.end(), sharedIdSizePtr, sharedIdSizePtr + sizeof(uint32_t));
			serializeData.insert(serializeData.end(), sharedId.begin(), sharedId.end());
		}
		else
		{
			serializeData.insert(serializeData.end(), value, value + uniformInfo.second.size);
		}
	}

	context->AddData(uniformLayout.size);
	context->AddData<uint32_t>(serializeData.size());
	context->AddData(serializeData);
}

bool Material::Deserialize(IDeserializeContext* context)
{
	isActive = context->Read<bool>();

	uint32_t resionSize = context->Read<uint32_t>();
	auto dataPtr = (int8_t*)context->Read(resionSize);
	auto serealizedPath = std::vector<int8_t>(dataPtr, dataPtr + resionSize);

	std::wstring shaderPath = AssetPath::DeserializePath(serealizedPath);
	const auto shader = Varlet::AssetRegister::Load(shaderPath);

	if (shader == nullptr)
		return false;

	SetShader(Cast<Shader>(shader));

	const auto& uniformLayout = _uniformLayouts[_shader];

	const uint32_t bufferSize = context->Read<uint32_t>();
	const uint32_t seriazelizedSize = context->Read<uint32_t>();

	if (bufferSize != uniformLayout.size)
		return false;

	for (const auto& uniformInfo : uniformLayout.uniformInfos)
	{
		const uint32_t typeId = context->Read<uint32_t>();
		const auto type = Varlet::Core::Type::GetType(typeId);

		assert(type);
		assert(type == uniformInfo.second.type);

		int8_t* dstValue = &uniformBuffer[0] + uniformInfo.second.offset;

		if (type->IsA(Texture))
		{
			resionSize = context->Read<uint32_t>();
			dataPtr = (int8_t*)context->Read(resionSize);
			auto data = std::vector<int8_t>(dataPtr, dataPtr + resionSize);

			if (data.empty())
			{
				memset(dstValue, 0, uniformInfo.second.size);
			}
			else
			{
				std::wstring texturePath = AssetPath::DeserializePath(data);
				*((int8_t**)dstValue) = (int8_t*)Varlet::AssetRegister::Load(texturePath);
			}
		}
		else
		{
			dataPtr = (int8_t*)context->Read(uniformInfo.second.size);
			memcpy(dstValue, dataPtr, uniformInfo.second.size);
		}
	}

	return true;
}

uint32_t Material::GetShaderTypeSize(Varlet::Graphics::ShaderDataType type)
{
	switch (type)
	{
	case Varlet::Graphics::ShaderDataType::Bool:		return sizeof(bool);
	case Varlet::Graphics::ShaderDataType::Int:			return sizeof(int32_t);
	case Varlet::Graphics::ShaderDataType::UInt:		return sizeof(uint32_t);
	case Varlet::Graphics::ShaderDataType::Float:		return sizeof(float);
	case Varlet::Graphics::ShaderDataType::Double:		return sizeof(double);
	case Varlet::Graphics::ShaderDataType::Vec2:		return sizeof(glm::vec2);
	case Varlet::Graphics::ShaderDataType::Vec3:		return sizeof(glm::vec3);
	case Varlet::Graphics::ShaderDataType::Vec4:		return sizeof(glm::vec4);
	case Varlet::Graphics::ShaderDataType::Mat2:		return sizeof(glm::mat2);
	case Varlet::Graphics::ShaderDataType::Mat3:		return sizeof(glm::mat3);
	case Varlet::Graphics::ShaderDataType::Mat4:		return sizeof(glm::mat4);
	case Varlet::Graphics::ShaderDataType::Sampler2D:	return sizeof(Texture*);
	}

	return 0;
}

const Varlet::Core::Type* Material::ShaderDataTypeToType(Varlet::Graphics::ShaderDataType type)
{
	static std::unordered_map<Varlet::Graphics::ShaderDataType, const Varlet::Core::Type*> cpuMappedTypes =
	{
		{ Varlet::Graphics::ShaderDataType::Bool,			Varlet::Core::Type::GetType(NAME_OF(bool))		},
		{ Varlet::Graphics::ShaderDataType::Int,			Varlet::Core::Type::GetType(NAME_OF(int32_t))	},
		{ Varlet::Graphics::ShaderDataType::UInt,			Varlet::Core::Type::GetType(NAME_OF(uint32_t))	},
		{ Varlet::Graphics::ShaderDataType::Float,			Varlet::Core::Type::GetType(NAME_OF(float))		},
		{ Varlet::Graphics::ShaderDataType::Double,			Varlet::Core::Type::GetType(NAME_OF(double))	},
		{ Varlet::Graphics::ShaderDataType::Vec2,			Varlet::Core::Type::GetType(NAME_OF(glm::vec2))	},
		{ Varlet::Graphics::ShaderDataType::Vec3,			Varlet::Core::Type::GetType(NAME_OF(glm::vec3))	},
		{ Varlet::Graphics::ShaderDataType::Mat2,			Varlet::Core::Type::GetType(NAME_OF(glm::mat2))	},
		{ Varlet::Graphics::ShaderDataType::Mat3,			Varlet::Core::Type::GetType(NAME_OF(glm::mat3))	},
		{ Varlet::Graphics::ShaderDataType::Mat4,			Varlet::Core::Type::GetType(NAME_OF(glm::mat4))	},
		{ Varlet::Graphics::ShaderDataType::Sampler2D,		Varlet::Core::Type::GetType(NAME_OF(Texture2D))	},
	};

	assert(cpuMappedTypes.contains(type));
	return cpuMappedTypes[type];
}

void Material::SetUniformValue(const char* name, const void* value)
{
	auto& layout = _uniformLayouts[_shader];

	if (layout.uniformInfos.contains(name))
	{
		const auto& info = layout.uniformInfos[name];
		memcpy(uniformBuffer.data() + info.offset, value, info.size);
	}
}
