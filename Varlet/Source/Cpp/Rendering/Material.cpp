#include "Material.h"
#include "Shader.h"

START_PROPERTY_BLOCK(Material)
PROPERTY("Activity", isActive)
for (auto uniform : _shader->uniforms)
TYPED_PROPERTY(uniform.name, uniform.value.get(), uniform.type);
END_PROPERTY_BLOCK;

Material::Material(Varlet::Shader* shader)
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
