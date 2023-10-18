#include "VarletModule.h"
#include "Reflection/StringType.h"
#include "Scene.h"

#define REGISTER_TYPE(TypeName) Varlet::Core::Type::Register(nullptr, new Varlet::Core::Type(#TypeName, sizeof(TypeName), { }, { }, nullptr, nullptr));

bool VarletModule::Initialize()
{
	REGISTER_TYPE(bool);
	REGISTER_TYPE(int8_t);
	REGISTER_TYPE(uint8_t);
	REGISTER_TYPE(int16_t);
	REGISTER_TYPE(uint16_t);
	REGISTER_TYPE(int32_t);
	REGISTER_TYPE(uint32_t);
	REGISTER_TYPE(int64_t);
	REGISTER_TYPE(uint64_t);
	REGISTER_TYPE(float);
	REGISTER_TYPE(double);

	Varlet::Core::Type::Register(nullptr, new Varlet::Core::StringType);

	REGISTER_TYPE(glm::bvec2);
	REGISTER_TYPE(glm::bvec3);
	REGISTER_TYPE(glm::bvec4);
	REGISTER_TYPE(glm::ivec2);
	REGISTER_TYPE(glm::ivec3);
	REGISTER_TYPE(glm::ivec4);
	REGISTER_TYPE(glm::uvec2);
	REGISTER_TYPE(glm::uvec3);
	REGISTER_TYPE(glm::uvec4);
	REGISTER_TYPE(glm::vec2);
	REGISTER_TYPE(glm::vec3);
	REGISTER_TYPE(glm::vec4);
	REGISTER_TYPE(glm::dvec2);
	REGISTER_TYPE(glm::dvec3);
	REGISTER_TYPE(glm::dvec4);
	REGISTER_TYPE(glm::mat2);
	REGISTER_TYPE(glm::mat3);
	REGISTER_TYPE(glm::mat4);

	Scene::Load(new Scene());

	return true;
}

void VarletModule::Update()
{
	Scene::Update();
}

void VarletModule::Shutdown()
{
	Scene::Get()->Shutdown();
}
