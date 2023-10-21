#include "RenderPipelineStages/BloomRenderPipelineStage.h"
#include "Assets/Shader.h"
#include "InternalCall.h"

BloomRenderPipelineStage::BloomRenderPipelineStage()
{
	_downSamplingShader = Shader::Create(WIDE("W:\\Varlet\\Modules\\GraphicsCore\\Assets\\Shaders\\Bloom\\DownSampling.vshader"), true);
	_upSamplingShader = Shader::Create(WIDE("W:\\Varlet\\Modules\\GraphicsCore\\Assets\\Shaders\\Bloom\\UpSampling.vshader"), true);
	_finalShader = Shader::Create(WIDE("W:\\Varlet\\Modules\\GraphicsCore\\Assets\\Shaders\\Bloom\\BloomFinal.vshader"), true);
	_emissionDetectShader = Shader::Create(WIDE("W:\\Varlet\\Modules\\GraphicsCore\\Assets\\Shaders\\Bloom\\EmissiveDetecting.vshader"), true);
}

void BloomRenderPipelineStage::Update()
{
	std::array<int8_t, sizeof(DownSamplingData)> uniforms;

	//	Emission Detect Pass

	memcpy(uniforms.data(), &_mainTexture, sizeof(void*));

	Varlet::Graphics::InternalCall::BindShader(_emissionDetectShader, nullptr, uniforms.data());
	Varlet::Graphics::InternalCall::DrawVertices(3);
	Varlet::Graphics::InternalCall::NextPass();

	glm::vec2 resolution = { 1920, 1080 };

	DownSamplingData downSamplingData;
	downSamplingData.sourceResolution = { 1920, 1080 };

	//	DownSampling Passes

	for (int32_t i = 0; i < _mipCount; i++)
	{
		downSamplingData.sourceTexture = i == 0
			? &_emissionColor
			: &mappedAttachments[i - 1];

		downSamplingData.mipLevel = i == 0 ? 0 : 1;

		memcpy(uniforms.data(), &downSamplingData, sizeof(DownSamplingData));

		Varlet::Graphics::InternalCall::BindShader(_downSamplingShader, nullptr, uniforms.data());
		Varlet::Graphics::InternalCall::DrawVertices(3);

		Varlet::Graphics::InternalCall::NextPass();

		downSamplingData.sourceResolution /= 2.f;
	}

	UpSamplingData upSamplingData;
	upSamplingData.filterRadius = _anotherFilterRadius;

	//	UpSampling Passes

	for (int32_t i = _mipCount - 1; i > 0; i--)
	{
		upSamplingData.sourceTexture = &mappedAttachments[i];

		memcpy(uniforms.data(), &upSamplingData.filterRadius, sizeof(float));
		memcpy(uniforms.data() + sizeof(float), &upSamplingData.sourceTexture, sizeof(void*));

		Varlet::Graphics::InternalCall::BindShader(_upSamplingShader, nullptr, uniforms.data());
		Varlet::Graphics::InternalCall::DrawVertices(3);

		Varlet::Graphics::InternalCall::NextPass();
	}

	FinalData finalData;
	finalData.exposure = exposure;
	finalData.bloomStrength = bloomStrength;
	finalData.t1 = &_initialColor;
	finalData.t2 = &mappedAttachments[0];

	memcpy(uniforms.data(), &finalData, sizeof(FinalData));

	Varlet::Graphics::InternalCall::BindShader(_finalShader, nullptr, uniforms.data());
	Varlet::Graphics::InternalCall::DrawVertices(3);
}

std::string BloomRenderPipelineStage::FormatShader(const std::string& source)
{
	return source;
}

Varlet::Graphics::RenderPipelineCreateInfo BloomRenderPipelineStage::GenerateCreateInfo(uint32_t width, uint32_t height, RenderPipelineStage* basePipeline)
{
	constexpr uint32_t MAX_MIP_COUNT = 7;

	uint32_t currentWidth = width;
	uint32_t currentHeight = height;

	_mainTexture = &basePipeline->mappedAttachments[0];

	std::vector<Varlet::Graphics::RenderTargetCreateInfo> renderTargets;
	renderTargets.reserve(MAX_MIP_COUNT + 2);
	mappedAttachments.resize(MAX_MIP_COUNT);

	renderTargets.push_back({
			currentWidth,
			currentHeight,
			TextureFormat::RGB111110F,
			&_initialColor
		});

	renderTargets.push_back({
			currentWidth,
			currentHeight,
			TextureFormat::RGB111110F,
			&_emissionColor
		});

	for (uint32_t i = 0; i < MAX_MIP_COUNT; i++)
	{
		currentWidth /= 2;
		currentHeight /= 2;

		renderTargets.push_back(
			{
				currentWidth,
				currentHeight,
				TextureFormat::RGB111110F,
				&mappedAttachments[i],
				FilterType::Linear
			});

		if (currentWidth < 2 || currentHeight < 2)
			break;
	}

	_mipCount = renderTargets.size() - 2;
	std::vector<Varlet::Graphics::RenderPassInfo> renderPasses;
	renderPasses.reserve(_mipCount * 2 + 1);

	// Emission dectect pass
	renderPasses.push_back({
		{ &_initialColor, &_emissionColor, }
		});

	// Downsamping passes
	for (int32_t i = 0; i < _mipCount; i++)
	{
		renderPasses.push_back({
				{ &mappedAttachments[i] },
			});
	}

	// Upsamping passes
	for (int32_t i = _mipCount - 1; i >= 0; i--)
	{
		void* attachment = i == 0
			? _mainTexture
			: &mappedAttachments[i - 1];

		renderPasses.push_back({
				{ attachment },
			});
	}

	return { std::move(renderTargets), std::move(renderPasses) };
}