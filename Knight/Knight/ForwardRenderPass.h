#pragma once

#include "SceneRenderPass.h"

class ForwardRenderPass : public LitRenderPass
{
	public:

		bool Create(Scene* sc) override;
		void Release() override;

		void BeginScene(SceneCamera* cam = NULL) override;
		void EndScene() override;

		Shader LightShader = { 0 };

		//CPU-side shader data for lights of the scene
		SceneLightShaderData _SceneLightData[NUM_MAX_LIGHTS] = { 0 };
};

//End of ForwardRenderPass.h
