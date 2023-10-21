#pragma once

#include "GraphicsCoreCore.h"
#include "Module.h"

class GRAPHICSCORE_API GraphicsCoreModule : public Varlet::Core::Module, public Varlet::Core::IUpdatebleModule
{
private:

	Varlet::Core::Module* _nativeGraphicsModule = nullptr;
	std::unordered_map<const Varlet::Core::Type*, std::vector<RenderPipelineStage*>> _pipelines;

public:

	~GraphicsCoreModule() override = default;

	bool Initialize() override;

	void Update() override;

	std::unordered_map<const Varlet::Core::Type*, std::vector<RenderPipelineStage*>>& GetAllRenderPipelineStages();

private:

	void OnNewRenderPipelineStageCreated(RenderPipelineStage* pipelineStage);

	void OnNewRenderPipelineStageRemoved(RenderPipelineStage* pipelineStage);
};
