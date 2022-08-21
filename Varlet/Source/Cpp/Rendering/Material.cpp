#include "Material.h"
#include "Shader.h"
#include "Texture.h"

START_PROPERTY_BLOCK(Material)
PROPERTY("Activity", isActive)
for (auto uniform : _shader->GetUniforms())
TYPED_PROPERTY(uniform.name, uniform.value.get(), uniform.type);
END_PROPERTY_BLOCK;

Material::Material(Varlet::Shader* shader)
{
	isActive = true;
	settings.stencilTest.enable = false;
	_shader = shader;

	for (const auto uniform : _shader->GetUniforms())
		if (uniform.type == Varlet::Type::Sampler2D || uniform.type == Varlet::Type::SamplerCube)
			_textures[uniform.name] = nullptr;
}

void Material::Activate() const
{
	assert(isActive);

	_shader->Use();

	int32_t unit = 0;
	for (const auto texture : _textures)
	{
		if (texture.second != nullptr)
			texture.second->Activate(unit);

		++unit;
	}

#ifdef META
	for (auto uniform : _shader->GetUniforms())
		SetUniform(uniform);
#endif // META
}

void Material::SetBool(const char* name, const bool& value) const
{
	_shader->SetBool(name, value);
}

void Material::SetUInt32(const char* name, const uint32_t& value) const
{
	_shader->SetUInt32(name, value);
}

void Material::SetInt32(const char* name, const int32_t& value) const
{
	_shader->SetInt32(name, value);
}

void Material::SetFloat(const char* name, const float& value) const
{
	_shader->SetFloat(name, value);
}

void Material::SetVec2(const char* name, const glm::vec2& value) const
{
	_shader->SetVec2(name, value);
}

void Material::SetVec3(const char* name, const glm::vec3& value) const
{
	_shader->SetVec2(name, value);
}

void Material::SetVec4(const char* name, const glm::vec4& value) const
{
	_shader->SetVec4(name, value);
}

void Material::SetMat3(const char* name, const glm::mat3& value) const
{
	_shader->SetMat3(name, value);
}

void Material::SetMat4(const char* name, const glm::mat4& value) const
{
	_shader->SetMat4(name, value);
}

void Material::SetSampler2D(const char* name, const std::shared_ptr<Varlet::Texture>& value)
{
	if (_textures.contains(name))
	{
		_textures[name] = value;
		_shader->SetUInt32(name, value->GetId());
	}
}

#ifdef META
void Material::SetUniform(const Uniform& uniform) const
{
#define UNIFORM_CASTED_VALUE(T) *static_cast<T*>(uniform.value.get())

	switch (uniform.type)
	{
	case Varlet::Type::Bool:
		_shader->SetBool(uniform.name, UNIFORM_CASTED_VALUE(bool));
		break;

	case Varlet::Type::Int32:
		_shader->SetInt32(uniform.name, UNIFORM_CASTED_VALUE(int32_t));
		break;

	case Varlet::Type::UInt32:
		_shader->SetUInt32(uniform.name, UNIFORM_CASTED_VALUE(uint32_t));
		break;

	case Varlet::Type::Float:
		_shader->SetFloat(uniform.name, UNIFORM_CASTED_VALUE(float));
		break;

	case Varlet::Type::Vector2:
		_shader->SetVec2(uniform.name, UNIFORM_CASTED_VALUE(glm::vec2));
		break;

	case Varlet::Type::Vector3:
	case Varlet::Type::Color3:
		_shader->SetVec3(uniform.name, UNIFORM_CASTED_VALUE(glm::vec3));
		break;

	case Varlet::Type::Vector4:
	case Varlet::Type::Color4:
		_shader->SetVec4(uniform.name, UNIFORM_CASTED_VALUE(glm::vec4));
		break;

	case Varlet::Type::Matrix3:
		_shader->SetMat3(uniform.name, UNIFORM_CASTED_VALUE(glm::mat3));
		break;

	case Varlet::Type::Matrix4:
		_shader->SetMat4(uniform.name, UNIFORM_CASTED_VALUE(glm::mat4));
		break;

	case Varlet::Type::Sampler2D:
		_shader->SetMat4(uniform.name, UNIFORM_CASTED_VALUE(glm::mat4));
		break;

	default:
		break;
	}
}
#endif // META
