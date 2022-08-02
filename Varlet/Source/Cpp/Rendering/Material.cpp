#include "Material.h"
#include "Shader.h"

Material::Material(const Varlet::Shader* shader)
{
	isActive = true;
	settings.stencilTest.enable = false;
	_shader = shader;
}

void Material::Activate() const
{
	assert(isActive);

	_shader->Use();
}

Varlet::Shader* Material::GetShader()
{
	return const_cast<Varlet::Shader*>(_shader);
}