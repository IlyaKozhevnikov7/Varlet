#include "RenderPipelineStages/ForwardRenderPipelineStage.h"
#include "InternalCall.h"
#include "../../../GraphicsInterface/Code/Include/GraphicsAPI.h"
#include "GraphicsBaseTypes.h"
#include "ShaderFormatter.h"

#include "Entity.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/PointLight.h"
#include "Assets/Material.h"

Varlet::Descriptor _illumination;

ForwardRenderPipelineStage::ForwardRenderPipelineStage()
{
	Renderer::OnRendererCreated.Bind(this, &ForwardRenderPipelineStage::OnRendererCreated);
	Renderer::OnRendererDestroyed.Bind(this, &ForwardRenderPipelineStage::OnRendererDestroyed);

	LightSource::OnLightSourceCreated.Bind(this, &ForwardRenderPipelineStage::OnLightSourceCreated);
	LightSource::OnLightSourceDestroyed.Bind(this, &ForwardRenderPipelineStage::OnLightSourceDestroyed);

	Varlet::Graphics::UniformBufferCreateInfo createInfo;
	createInfo.bindingName = "Illumination";
	createInfo.size = sizeof(Varlet::Graphics::IlluminationData);
	createInfo.usage = Varlet::Graphics::BufferUsage::Uniform;

	Varlet::Graphics::CreateUniformBuffer(&_illumination, createInfo);

	for (auto entity : Scene::GetAllEntities())
		for (auto component : entity->GetComponents())
			NewComponentCreated(entity, component);
}

void ForwardRenderPipelineStage::Update()
{
	UpdateIllumination();

	for (auto renderer : _renderers)
	{
		if (renderer->isVisible == false)
			continue;
	
		auto mesh = renderer->GetMesh();

		if (renderer->material == nullptr || mesh == nullptr)
			continue;
	
		renderer->material->SetMat4("u_View", *cameraInfo.view);
		renderer->material->SetMat4("u_Projection", *cameraInfo.projection);
		renderer->material->SetMat4("u_Model", renderer->GetModelMatrix());
		renderer->material->SetVec3("u_CameraPosition", *cameraInfo.position);

		const auto& uniformBuffer = Material::ReceiveUniformBuffer(renderer->material);
		Varlet::Graphics::BindShader(renderer->material->GetShader(), (void*)GetTypeStatic(), uniformBuffer.data());

		Varlet::Graphics::InternalCall::Draw(mesh);
	}
}

std::string ForwardRenderPipelineStage::FormatShader(const std::string& source)
{
#define POSITION_ASSEMPLY_NAME	"POSITION"
#define NORMAL_ASSEMPLY_NAME	"NORMAL"
#define UV_ASSEMPLY_NAME		"UV"
#define TANGENTS_ASSEMPLY_NAME	"TANGENT"

	Varlet::Graphics::ShaderFormatter formatter(source);

	formatter.AddVertextAttribute(POSITION_ASSEMPLY_NAME, Varlet::Graphics::ShaderDataType::Vec3);
	formatter.AddVertextAttribute(NORMAL_ASSEMPLY_NAME, Varlet::Graphics::ShaderDataType::Vec3);
	formatter.AddVertextAttribute(UV_ASSEMPLY_NAME, Varlet::Graphics::ShaderDataType::Vec2);
	formatter.AddVertextAttribute(TANGENTS_ASSEMPLY_NAME, Varlet::Graphics::ShaderDataType::Vec3);

	return formatter.GetSource();
}

Varlet::Graphics::RenderPipelineCreateInfo ForwardRenderPipelineStage::GenerateCreateInfo(uint32_t width, uint32_t height, RenderPipelineStage* basePipeline)
{
	mappedAttachments.resize(1);

	return
	{
		// Render targets
		{
			{ width, height, TextureFormat::RGB111110F, &mappedAttachments[0] },	// Color
			{ width, height, TextureFormat::D24S8, nullptr }						// Depth-Stencil
		},

		// Render Passes
		{
			{ 
				{ &mappedAttachments[0] },
			}
		}
	};
}

void ForwardRenderPipelineStage::UpdateIllumination()
{
	Varlet::Graphics::IlluminationData illuminationData;
	memset(illuminationData.pointLights.data(), 0, sizeof(Varlet::Graphics::IlluminationData));

	for (uint32_t i = 0; i < _pointLightSets.size() && i < Varlet::Graphics::IlluminationData::MAX_POINT_LIGHT_COUNT; i++)
	{
		auto currentLightData = _pointLightSets[i];

		illuminationData.pointLights[i].isActive = currentLightData.pointLight->IsActive();

		if (illuminationData.pointLights[i].isActive == false)
			continue;

		illuminationData.pointLights[i].color = currentLightData.pointLight->color;
		illuminationData.pointLights[i].position = currentLightData.transform->position;
		illuminationData.pointLights[i].intensity = currentLightData.pointLight->intensity;
		illuminationData.pointLights[i].attenuation = currentLightData.pointLight->attenuation;
	}

	Varlet::Graphics::UpdateUniformBufferData(&_illumination, &illuminationData, sizeof(Varlet::Graphics::IlluminationData), 0);
}

void ForwardRenderPipelineStage::NewComponentCreated(Entity* entity, Component* component)
{
	if (const auto renderer = Cast<Renderer>(component))
	{
		_renderers.push_back(renderer);
	}
	else if (const auto pointLight = Cast<PointLight>(component))
	{
		const auto transform = entity->GetComponent<Transform>();
		assert(transform);

		_pointLightSets.push_back({ pointLight, transform });
	}
}

void ForwardRenderPipelineStage::OnRendererCreated(Renderer* renderer)
{
	assert(renderer);

	_renderers.push_back(renderer);
}

void ForwardRenderPipelineStage::OnRendererDestroyed(Renderer* renderer)
{
	assert(renderer);

	const auto it = std::find(_renderers.begin(), _renderers.end(), renderer);

	if (it != _renderers.end())
		_renderers.erase(it);
}

void ForwardRenderPipelineStage::OnLightSourceCreated(LightSource* lightSource)
{
	assert(lightSource);

	const auto transform = lightSource->GetOwner()->GetComponent<Transform>();
	assert(transform);

	if (const auto pointLight = Cast<PointLight>(lightSource))
	{
		_pointLightSets.push_back({ pointLight , transform });
	}	
}

void ForwardRenderPipelineStage::OnLightSourceDestroyed(LightSource* lightSource)
{
	assert(lightSource);

	const auto it = std::find_if(_pointLightSets.begin(), _pointLightSets.end(), [lightSource](const Varlet::Graphics::PointLightSet& entry)
		{
			return entry.pointLight == lightSource;
		});

	if (it != _pointLightSets.end())
		_pointLightSets.erase(it);
}