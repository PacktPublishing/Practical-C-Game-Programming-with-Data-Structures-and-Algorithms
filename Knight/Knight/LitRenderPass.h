#pragma once

#include "SceneRenderPass.h"

class LitRenderPass : public SceneRenderPass
{
public:

	//CPU-side shader data for lights of the scene
	SceneLightShaderData _SceneLightData[NUM_MAX_LIGHTS] = { 0 };
	//Knight use a global ambient color for the scene
	int ambientLoc = -1;
	//Material shininess uniform location
	int shinenessLoc = -1;

protected:
	//Get uniform location for scene light data
	void InitRenderPassUniforms(const Shader&) override;
	virtual void UpdateLightData(const Shader&);
};

//End of LitRenderPass.h
