#include "EditorUtils.h"
#include "VarletFramework.h"
#include "Rendering/Texture.h"
#include "Rendering/RendererAPI.h"

Entity* EditorUtils::CreatePointLight()
{
	auto light = Scene::CreateEntity();

	light->AddComponent<Transform>();
	light->AddComponent<PointLight>();
	auto textureRenderer = light->AddComponent<TextureRenderer>();

	const auto defautlTextureShader = Varlet::RendererAPI::CreateShader({
		"W:/Varlet/Varlet/Shaders/defaultTextureVertex.glsl",
		"W:/Varlet/Varlet/Shaders/defaultTextureFragment.glsl",
		"" });
	
	textureRenderer->materials.push_back(new Material(defautlTextureShader));

	auto texture = Varlet::Texture::Load("W:\\Varlet\\Sandbox\\Assets\\PointLight.png");
	textureRenderer->SetTexture(texture);

	return light;
}
