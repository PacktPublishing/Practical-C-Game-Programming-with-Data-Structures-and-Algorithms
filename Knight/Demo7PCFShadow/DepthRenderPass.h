#pragma once

#include "Knight.h"

#include "ShadowSceneLight.h"

class DepthRenderPass : public SceneRenderPass
{
public:

	DepthRenderPass(ShadowSceneLight* l);

	bool Create(Scene* sc) override;
	void Release() override;

	void BeginScene(SceneCamera* cam = NULL) override;
	void EndScene() override;
	bool OnAddToRender(Component* pSC, SceneObject* pSO) override;

	void BeginShadowMap();
	void EndShadowMap();

	RenderTexture2D shadowMap = { 0 };
	int shadowMapResolution = 1024;

	Shader depthShader = { 0 };

	ShadowSceneLight* pLight = nullptr;

protected:

	RenderTexture2D LoadShadowmapRenderTexture(int width, int height);
	void UnloadShadowmapRenderTexture(RenderTexture2D target);
};

