#include "EditorUtils.h"
#include "VarletFramework.h"

Entity* EditorUtils::CreatePointLight()
{
	auto light = Scene::CreateEntity();

	light->AddComponent<Transform>();
	light->AddComponent<PointLight>();
	auto textureRenderer = light->AddComponent<TextureRenderer>();

	LoadableTextureConfiguration loadableTextureConfiguration;
	loadableTextureConfiguration.path = "W:\\Varlet\\Sandbox\\Assets\\PointLight.png";
	loadableTextureConfiguration.wrapType = WrapType::ClampToEdge;

	const auto defautlTextureShader = Varlet::RendererAPI::CreateShader({
		"W:/Varlet/Varlet/Shaders/defaultTextureVertex.glsl",
		"W:/Varlet/Varlet/Shaders/defaultTextureFragment.glsl",
		"" });

	textureRenderer->materials.push_back(new Material(defautlTextureShader));

	auto texture = Varlet::RendererAPI::LoadTexture(loadableTextureConfiguration);
	textureRenderer->SetTexture(texture);

	return light;
}
