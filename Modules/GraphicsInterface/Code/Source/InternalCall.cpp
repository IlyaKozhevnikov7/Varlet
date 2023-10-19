#include "InternalCall.h"

namespace Varlet::Graphics
{
	CreateShaderSignature InternalCall::CreateShader = nullptr;
	CreateTextureSignature InternalCall::CreateTexture = nullptr;
	CreateModelSignature InternalCall::CreateModel = nullptr;
	CreateUniformBufferSignature InternalCall::CreateUniformBuffer = nullptr;
	CreateRenderPipelineSignature InternalCall::CreateRenderPipeline = nullptr;

	BeginRenderPipelineSignature  InternalCall::BeginRenderPipeline = nullptr;
	EndRenderPipelineSignature InternalCall::EndRenderPipeline = nullptr;
	NextPassSignature InternalCall::NextPass = nullptr;
	BindShaderSignature InternalCall::BindShader = nullptr;
	DrawSignature InternalCall::Draw = nullptr;
	DrawVerticesSignature InternalCall::DrawVertices = nullptr;

	GetNativeTextureSignature InternalCall::GetNativeTexture = nullptr;
	GetShaderUniformInfosSignature InternalCall::GetShaderUniformInfos = nullptr;
	UpdateUniformBufferDataSignature InternalCall::UpdateUniformBufferData = nullptr;
}
