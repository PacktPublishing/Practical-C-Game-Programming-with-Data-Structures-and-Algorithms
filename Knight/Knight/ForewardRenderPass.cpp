#include "Knight.h"

#include "rlgl.h"

bool ForwardRenderPass::Create(Scene* sc)
{
	__super::Create(sc);

	//default forward rendering pipeline use simple lighting model
	LightShader = LoadShader("../../resources/shaders/glsl330/kn_lit.vs", "../../resources/shaders/glsl330/kn_lit.fs");
	InitLightUniforms(LightShader);
	alphaTestLoc = GetShaderLocation(LightShader, "alphaTest");

	//Note: rlight of Raylib use a hardcoded value of 16.0f for shininess in the 
	//this value should be overridden by the material, if not set, we provided a default value same as Raylib's rlight module 
	SetShaderValue(LightShader, shinenessLoc, &pScene->DefaultShineness, SHADER_UNIFORM_FLOAT);
	int alphaTestDefault = 0;
	SetShaderValue(LightShader, alphaTestLoc, &alphaTestDefault, SHADER_UNIFORM_INT);

	Hints.pOverrideShader = &LightShader;

	return true;
}

void ForwardRenderPass::Release()
{
}

void ForwardRenderPass::BeginScene(SceneCamera *pOverrideCamera)
{
	UpdateLightData(LightShader);

	pActiveCamera = pScene->GetMainCameraActor();
	if (pOverrideCamera != nullptr)
		pActiveCamera = pOverrideCamera;
	pScene->_CurrentRenderPass = this;
	pScene->ClearRenderQueue();
	BuildRenderQueue(pScene->SceneRoot);	
}

void ForwardRenderPass::EndScene()
{
	pActiveCamera = NULL;
	pScene->_CurrentRenderPass = nullptr;
}


