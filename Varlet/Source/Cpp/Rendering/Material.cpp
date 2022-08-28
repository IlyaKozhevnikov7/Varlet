#include "Material.h"
#include "Shader.h"
#include "Texture.h"

START_PROPERTY_BLOCK(Material)
PROPERTY("Activity", isActive)
for (int32_t i = 0; i < _shader->_uniformDeclarations.size(); i++)
TYPED_PROPERTY(_shader->_uniformDeclarations[i].first, _uniformValues[i], _shader->_uniformDeclarations[i].second);
END_PROPERTY_BLOCK;

Material::Material(Varlet::Shader* shader)
{
	isActive = true;
	settings.stencilTest.enable = false;
	SetShader(shader);
}

void Material::Activate() const
{
	assert(isActive);

	_shader->Use();

#ifdef META
	for (int32_t i = 0; i < _shader->_uniformDeclarations.size(); i++)
		SetUniform(_shader->_uniformDeclarations[i].first, _uniformValues[i], _shader->_uniformDeclarations[i].second);
#endif // META

	BindTextures();
}

void Material::SetShader(Varlet::Shader* newShader)
{
	if (_shader == newShader)
		return;

	_shader = newShader;

	for (int32_t i = 0; i < newShader->_uniformDeclarations.size(); i++)
	{
		if (newShader->_uniformDeclarations[i].second == Varlet::Type::Sampler2D || newShader->_uniformDeclarations[i].second == Varlet::Type::SamplerCube)
			_textures[newShader->_uniformDeclarations[i].first] = nullptr;

#ifdef META
#define SET_UNIFORM_DATA(T, Value) _uniformValues.push_back(new T(Value)); break;

		switch (newShader->_uniformDeclarations[i].second)
		{
		case Varlet::Type::Bool: SET_UNIFORM_DATA(bool, true);
		case Varlet::Type::Int32: SET_UNIFORM_DATA(int32_t, 0);
		case Varlet::Type::UInt32: SET_UNIFORM_DATA(uint32_t, 0);
		case Varlet::Type::Float: SET_UNIFORM_DATA(float, 0);
		case Varlet::Type::Double: SET_UNIFORM_DATA(double, 0);
		case Varlet::Type::BoolVector2: SET_UNIFORM_DATA(glm::bvec2, true);
		case Varlet::Type::BoolVector3: SET_UNIFORM_DATA(glm::bvec3, true);
		case Varlet::Type::BoolVector4: SET_UNIFORM_DATA(glm::bvec4, true);
		case Varlet::Type::Int32Vector2: SET_UNIFORM_DATA(glm::ivec2, 1);
		case Varlet::Type::Int32Vector3: SET_UNIFORM_DATA(glm::ivec3, 1);
		case Varlet::Type::Int32Vector4: SET_UNIFORM_DATA(glm::ivec4, 1);
		case Varlet::Type::UInt32Vector2: SET_UNIFORM_DATA(glm::uvec2, 1);
		case Varlet::Type::UInt32Vector3: SET_UNIFORM_DATA(glm::uvec3, 1);
		case Varlet::Type::UInt32Vector4: SET_UNIFORM_DATA(glm::uvec4, 1);
		case Varlet::Type::Vector2: SET_UNIFORM_DATA(glm::vec2, 1);
		case Varlet::Type::Vector3:
		case Varlet::Type::Color3: SET_UNIFORM_DATA(glm::vec3, 1);
		case Varlet::Type::Vector4:
		case Varlet::Type::Color4: SET_UNIFORM_DATA(glm::vec4, 1);
		case Varlet::Type::DoubleVector2: SET_UNIFORM_DATA(glm::dvec2, 1);
		case Varlet::Type::DoubleVector3: SET_UNIFORM_DATA(glm::dvec3, 1);
		case Varlet::Type::DoubleVector4: SET_UNIFORM_DATA(glm::dvec4, 1);
		case Varlet::Type::Matrix2: SET_UNIFORM_DATA(glm::mat2, 1);
		case Varlet::Type::Matrix3: SET_UNIFORM_DATA(glm::mat3, 1);
		case Varlet::Type::Matrix4: SET_UNIFORM_DATA(glm::mat4, 1);
		case Varlet::Type::Sampler2D:
		case Varlet::Type::SamplerCube: _uniformValues.push_back(nullptr); break;
		default: _uniformValues.push_back(nullptr); break;
		}
#endif // META
	}
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
	_shader->SetVec3(name, value);
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

void Material::SetSampler2D(const char* name, Varlet::Texture* value) const
{
	if (_textures.contains(name))
	{
		_textures[name] = value;

#ifdef META
		for (int32_t i = 0; i < _shader->_uniformDeclarations.size(); i++)
			if (_shader->_uniformDeclarations[i].second == Varlet::Type::Sampler2D && !strcmp(_shader->_uniformDeclarations[i].first, name))
			{
				_uniformValues[i] = value;
				return;
			}
#endif // META
	}
}

void Material::BindTextures() const
{
	int32_t unit = 0;
	for (const auto& texture : _textures)
	{
		if (texture.second != nullptr)
			texture.second->Activate(unit);
		else
			Varlet::RendererAPI::UnbindTexure(unit);
			
		++unit;
	}
}

#ifdef META
void Material::SetUniform(const char* name, void* value, const Varlet::Type& type) const
{
#define UNIFORM_CASTED_VALUE(T) *static_cast<T*>(value)

	switch (type)
	{
	case Varlet::Type::Bool:
		_shader->SetBool(name, UNIFORM_CASTED_VALUE(bool));
		break;

	case Varlet::Type::Int32:
		_shader->SetInt32(name, UNIFORM_CASTED_VALUE(int32_t));
		break;

	case Varlet::Type::UInt32:
		_shader->SetUInt32(name, UNIFORM_CASTED_VALUE(uint32_t));
		break;

	case Varlet::Type::Float:
		_shader->SetFloat(name, UNIFORM_CASTED_VALUE(float));
		break;

	case Varlet::Type::Vector2:
		_shader->SetVec2(name, UNIFORM_CASTED_VALUE(glm::vec2));
		break;

	case Varlet::Type::Vector3:
	case Varlet::Type::Color3:
		_shader->SetVec3(name, UNIFORM_CASTED_VALUE(glm::vec3));
		break;

	case Varlet::Type::Vector4:
	case Varlet::Type::Color4:
		_shader->SetVec4(name, UNIFORM_CASTED_VALUE(glm::vec4));
		break;

	case Varlet::Type::Matrix3:
		_shader->SetMat3(name, UNIFORM_CASTED_VALUE(glm::mat3));
		break;

	case Varlet::Type::Matrix4:
		_shader->SetMat4(name, UNIFORM_CASTED_VALUE(glm::mat4));
		break;

	case Varlet::Type::Sampler2D:
	{
		if (_textures.contains(name))
			_textures[name] = static_cast<Varlet::Texture*>(value);
	}
	break;

	default:
		break;
	}
}
#endif // META
