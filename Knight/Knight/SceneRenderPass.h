#pragma once

#include <set>
#include <vector>

#include "Scene.h"

typedef struct 
{
	int enabledLoc;
	int typeLoc;
	int positionLoc;
	int targetLoc;
	int colorLoc;
	int attnConstLoc;
	int attnLinearLoc;
	int attnQuadLoc;
} SceneLightShaderData;

class SceneRenderPass
{
	public:

		virtual bool Create(Scene *sc) = 0;
		virtual void Release() = 0;

		virtual void BeginScene(SceneCamera *cam = nullptr) = 0;
		virtual void Render();
		virtual void EndScene() = 0;

		virtual void BuildRenderQueue(SceneObject *pR);
		virtual bool OnAddToRender(Component* pSC, SceneObject* pSO);

		RenderHints Hints = { 0 };

		// _Priority controls the order in which render passes are executed.
		int _Priority = 0;

		//CPU-side material shader unform data for all knight shaders

		int alphaTestLoc = -1;

		//The active camera used for this render pass,
		//This can be the Scene's main camera or an override camera, light a light's camera for shadow map rendering
		SceneCamera* pActiveCamera = nullptr;

		//Number of components skipped due to frustum culling
		int NumComponentsSkipped = 0;

	protected:

		Scene* pScene = nullptr;

		//Get uniform location shader
		virtual void InitRenderPassUniforms(const Shader&);

		virtual void EnableAlphaTest(bool enable)
		{
			if (Hints.pOverrideShader != nullptr && alphaTestLoc >= 0)
			{
				int alphaTestValue = enable ? 1 : 0;
				SetShaderValue(*Hints.pOverrideShader, alphaTestLoc, &alphaTestValue, SHADER_UNIFORM_INT);
			}
		}
};

//End of SceneRenderPass.h